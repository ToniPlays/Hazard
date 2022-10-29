
#include "OpenGLShader.h"
#ifdef HZR_INCLUDE_OPENGL

#include "../OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"
#include "Backend/OpenGL/OpenGLShaderCompiler.h"
#include "../Textures/OpenGLImage2D.h"
#include "../OpenGLContext.h"
#include "OpenGLBuffers.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::OpenGL
{
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_ASSERT(!filePath.empty(), "Shader path cannot be empty");
		Reload();
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}
	void OpenGLShader::Reload()
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;

		std::unordered_map<ShaderStage, Buffer> vulkanBinaries;
		std::unordered_map<ShaderStage, Buffer> openGLbinaries;

		if (ShaderFactory::HasCachedShader(m_FilePath, RenderAPI::Vulkan) == CacheStatus::Exists)
		{
			vulkanBinaries = ShaderFactory::GetShaderBinaries(m_FilePath, RenderAPI::Vulkan);
			openGLbinaries = ShaderFactory::GetShaderBinaries(m_FilePath, RenderAPI::OpenGL);
		}
		else
		{
			std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(m_FilePath);

			//Compile OpenGL shader
			OpenGLShaderCompiler compiler;

			for (auto& [stage, source] : sources)
			{
				double compilationTime = 0.0;
				std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

				//Compile to Vulkan SPV for reflection
				CompileInfo compileInfoVulkan = {};
				compileInfoVulkan.Renderer = RenderAPI::Vulkan;
				compileInfoVulkan.Name = m_FilePath;
				compileInfoVulkan.Stage = stage;
				compileInfoVulkan.Source = source;
				compileInfoVulkan.Defines = defines;

				if (!compiler.Compile(&compileInfoVulkan))
				{
					Window::SendDebugMessage({
						Severity::Warning,
						fmt::format("{0} failed to compile (Vulkan):\n - {0}", compiler.GetErrorMessage()),
						source
						});
					continue;
				}

				compilationTime += compiler.GetCompileTime();
				vulkanBinaries[stage] = Buffer::Copy(compiler.GetCompiledBinary());

				std::vector<ShaderDefine> glDefines = { { "OPENGL_API" } };

				//Compile to Vulkan SPV for OpenGL source
				CompileInfo compileInfoVkToGL = {};
				compileInfoVkToGL.Renderer = RenderAPI::Vulkan;
				compileInfoVkToGL.Name = m_FilePath;
				compileInfoVkToGL.Stage = stage;
				compileInfoVkToGL.Source = source;
				compileInfoVkToGL.Defines = glDefines;

				if (!compiler.Compile(&compileInfoVkToGL))
				{
					Window::SendDebugMessage({
						Severity::Warning,
						fmt::format("{0} failed to compile (Vk to OpenGL):\n - {1}", m_FilePath, compiler.GetErrorMessage()),
						source
						});
					continue;
				}
				compilationTime += compiler.GetCompileTime();

				//Get OpenGL shader source from Vulkan binaries
				std::string glSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
				{
					Window::SendDebugMessage({
						Severity::Warning,
						fmt::format("{0} failed to decompile:\n - {1}", m_FilePath, compiler.GetErrorMessage()),
						source
						});
					continue;
				}

				//Compile to OpenGL SPV
				CompileInfo compileInfoOpenGL = {};
				compileInfoOpenGL.Renderer = RenderAPI::OpenGL;
				compileInfoOpenGL.Name = m_FilePath;
				compileInfoOpenGL.Stage = stage;
				compileInfoOpenGL.Source = glSource;
				compileInfoOpenGL.Defines = glDefines;

				//Get OpenGL compiled binary
				if (!compiler.Compile(&compileInfoOpenGL))
				{
					Window::SendDebugMessage({
						Severity::Warning,
						fmt::format("{0} failed to compile (OpenGL):\n - {1}", m_FilePath, compiler.GetErrorMessage()),
						glSource
						});
					continue;
				}

				compilationTime += compiler.GetCompileTime();
				openGLbinaries[stage] = Buffer::Copy(compiler.GetCompiledBinary());
			}

			ShaderFactory::CacheShader(m_FilePath, vulkanBinaries, RenderAPI::Vulkan);
			ShaderFactory::CacheShader(m_FilePath, openGLbinaries, RenderAPI::OpenGL);
		}

		m_ShaderData.Stages.clear();
		ReflectVulkan(vulkanBinaries);
		Reflect(openGLbinaries);

		Ref<OpenGLShader> instance = this;
		Renderer::SubmitResourceCreate([instance, vulkanBinaries, openGLbinaries]() mutable {
			instance->CreateProgram(openGLbinaries);

			for (auto& [stage, buffer] : vulkanBinaries)
				buffer.Release();
			for (auto& [stage, buffer] : openGLbinaries)
				buffer.Release();
			});

		Window::SendDebugMessage({ Severity::Info, fmt::format("Shader {0} loaded", m_FilePath), fmt::format("Shader reload took {0} ms", timer.ElapsedMillis()) });
	}
	bool OpenGLShader::SetUniformBuffer(uint32_t set, uint32_t binding, void* data, uint32_t size)
	{
		m_DescriptorSet[set].GetWriteDescriptor(binding).BoundValue[0].As<UniformBuffer>()->SetData(data, size);
		return true;
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, Ref<Image2D> image)
	{
		for (auto& [set, descriptor] : m_DescriptorSet)
		{
			auto& write = descriptor.GetWriteDescriptor(name);
			if (write.ActualBinding == UINT32_MAX)
				continue;

			write.BoundValue[index] = image;
			return;
		}
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap)
	{
		for (auto& [set, descriptor] : m_DescriptorSet)
		{
			auto& write = descriptor.GetWriteDescriptor(name);
			if (write.ActualBinding == UINT32_MAX)
				continue;

			write.BoundValue[index] = cubemap;
			return;
		}
	}
	void OpenGLShader::Reflect(const std::unordered_map<ShaderStage, Buffer>& binaries)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		OpenGLShaderCompiler compiler;
		ShaderData data = compiler.GetShaderResources(binaries);

		for (auto& [set, descriptor] : m_DescriptorSet)
		{
			//Check single descriptor set
			for (auto& [binding, writeDescriptor] : descriptor.GetDescriptorSets())
			{
				for (auto& [uboSet, uniformBuffers] : data.UniformsDescriptions)
				{
					for (auto& [bufferBinding, buffer] : uniformBuffers)
					{
						if (writeDescriptor.DebugName == buffer.Name)
							writeDescriptor.ActualBinding = bufferBinding;
					}
				}
				for (auto& [samplerSet, samplers] : data.ImageSamplers)
				{
					for (auto& [samplerBinding, sampler] : samplers)
					{
						if (writeDescriptor.DebugName == sampler.Name)
							writeDescriptor.ActualBinding = samplerBinding;
					}
				}
			}
		}
		//OpenGLShaderCompiler::PrintReflectionData(data);
	}
	void OpenGLShader::Reload_RT(std::unordered_map<ShaderStage, Buffer> binaries)
	{
		if (m_ID)
			glDeleteProgram(m_ID);
		CreateProgram(binaries);
	}
	static void CheckShader(GLuint id, GLuint type)
	{
		HZR_PROFILE_FUNCTION();
		GLint ret = 0;
		//Check if something is wrong with the shader
		switch (type) {
		case(GL_COMPILE_STATUS):
			glGetShaderiv(id, type, &ret);
			if (ret == false) {
				int infologLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				std::vector<GLchar> infoLog(infologLength);
				GLsizei charsWritten = 0;

				glGetShaderInfoLog(id, infologLength, &charsWritten, infoLog.data());
				Window::SendDebugMessage({ Severity::Error, "Shader compilation failed", std::string(infoLog.data()) });
			}
			break;
		case(GL_LINK_STATUS):
			glGetProgramiv(id, type, &ret);
			if (ret == false) {
				int infologLength = 0;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				std::vector<GLchar> infoLog(infologLength);
				GLsizei charsWritten = 0;

				glGetProgramInfoLog(id, infologLength, &charsWritten, infoLog.data());
				std::string message;

				if (charsWritten)
				{
					message = std::string(infoLog.data());
				}
				Window::SendDebugMessage({ Severity::Error, "Shader linking failed", message });
			}
			break;
		default:
			break;
		};
	}

	void OpenGLShader::CreateProgram(const std::unordered_map<ShaderStage, Buffer>& binary)
	{
		HZR_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : binary)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(OpenGLUtils::ShaderStageToGLType(stage)));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.Data, spirv.Size);
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
			CheckShader(program, shaderID);
		}
		glLinkProgram(program);

		CheckShader(program, GL_LINK_STATUS);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (linked == GL_FALSE)
		{
			GLint maxLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);

			std::vector<GLchar> infoLog(maxLen);
			glGetShaderInfoLog(program, maxLen, &maxLen, infoLog.data());

			std::string message;

			if (maxLen > 0)
			{
				message = std::string(infoLog.data());
			}

			Window::SendDebugMessage({ Severity::Error, "Shader linking failed", message });
			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_ID = program;
	}
	void OpenGLShader::ReflectVulkan(const std::unordered_map<ShaderStage, Buffer>& binaries)
	{
		Timer timer;
		OpenGLShaderCompiler compiler;
		m_ShaderData = compiler.GetShaderResources(binaries);

		for (auto& [set, buffers] : m_ShaderData.UniformsDescriptions)
		{
			OpenGLDescriptorSet& descriptorSet = m_DescriptorSet[set];
			for (auto& [binding, buffer] : buffers)
			{
				OpenGLWriteDescriptor writeDescriptor = {};
				writeDescriptor.Type = GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				writeDescriptor.DebugName = buffer.Name;
				writeDescriptor.Binding = binding;
				writeDescriptor.ArraySize = 0;

				descriptorSet.AddWriteDescriptor(writeDescriptor);

				UniformBufferCreateInfo bufferInfo = {};
				bufferInfo.Name = buffer.Name;
				bufferInfo.Set = set;
				bufferInfo.Binding = binding;
				bufferInfo.Size = buffer.Size;
				bufferInfo.IsShared = true;
				bufferInfo.Usage = BufferUsage::DynamicDraw;

				descriptorSet.GetWriteDescriptor(binding).BoundValue[0] = UniformBuffer::Create(&bufferInfo);
			}
		}
		for (auto& [set, samplers] : m_ShaderData.ImageSamplers)
		{
			OpenGLDescriptorSet& descriptorSet = m_DescriptorSet[set];
			for (auto& [binding, sampler] : samplers)
			{
				OpenGLWriteDescriptor writeDescriptor = {};
				writeDescriptor.Type = GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				writeDescriptor.DebugName = sampler.Name;
				writeDescriptor.Binding = binding;
				writeDescriptor.ArraySize = sampler.ArraySize;
				writeDescriptor.Dimension = sampler.Dimension;

				descriptorSet.AddWriteDescriptor(writeDescriptor);

				for (uint32_t i = 0; i < sampler.ArraySize; i++)
					if (sampler.Dimension == 2)
						Set(sampler.Name, i, OpenGLContext::GetInstance().GetDefaultResources().WhiteTexture);
			}
		}
		for (auto& [set, storageImage] : m_ShaderData.StorageImages)
		{
			OpenGLDescriptorSet& descriptorSet = m_DescriptorSet[set];
			for (auto& [binding, image] : storageImage)
			{
				OpenGLWriteDescriptor writeDescriptor = {};
				writeDescriptor.Type = GL_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				writeDescriptor.DebugName = image.Name;
				writeDescriptor.Binding = binding;
				writeDescriptor.ArraySize = image.ArraySize;
				writeDescriptor.ActualBinding = binding;
				writeDescriptor.Dimension = image.Dimension;

				descriptorSet.AddWriteDescriptor(writeDescriptor);
			}
		}
	}
}
#endif

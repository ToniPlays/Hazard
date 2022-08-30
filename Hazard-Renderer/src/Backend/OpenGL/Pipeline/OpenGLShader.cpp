
#include "OpenGLShader.h"
#ifdef HZR_INCLUDE_OPENGL

#include "../OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"
#include "Backend/OpenGL/OpenGLShaderCompiler.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

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
		Timer timer;
		if (ShaderFactory::HasCachedShader(ShaderStage::Vertex, m_FilePath))
		{
			__debugbreak();
			return;
		}

		std::cout << m_FilePath << std::endl;
		std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(m_FilePath);

		//Compile OpenGL shader
		OpenGLShaderCompiler compiler;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> vulkanBinaries;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> openGLbinaries;

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
				std::cout << compiler.GetErrorMessage() << std::endl;
				__debugbreak();
				continue;
			}
			compilationTime += compiler.GetCompileTime();
			vulkanBinaries[stage] = compiler.GetCompiledBinary();

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
				std::cout << compiler.GetErrorMessage() << std::endl;
				__debugbreak();
				continue;
			}
			compilationTime += compiler.GetCompileTime();

			//Get OpenGL shader source from Vulkan binaries
			std::string glSource;
			if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
			{
				std::cout << "Decompilation failed" << std::endl;
				__debugbreak();
				continue;
			}

			//std::cout << glSource << std::endl;

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
				std::cout << "Decompilation failed" << std::endl;
				__debugbreak();
				continue;
			}

			compilationTime += compiler.GetCompileTime();
			openGLbinaries[stage] = compiler.GetCompiledBinary();
			//std::cout << "Compilation took: " << compilationTime << "ms" << std::endl;
		}

		m_ShaderData.Stages.clear();
		ReflectVulkan(vulkanBinaries);
		Reflect(openGLbinaries);

		Ref<OpenGLShader> instance = this;
		Renderer::Submit([instance, openGLbinaries]() mutable {
			instance->CreateProgram(openGLbinaries);
			});

		std::cout << "Shader reload took " << timer.ElapsedMillis() << "ms\n" << std::endl;
	}
	bool OpenGLShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		/*
		auto& uniformBuffer = m_UniformBuffers[name];
		if (!uniformBuffer) return false;
		//HZR_ASSERT(uniformBuffer, "[OpenGLShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
		*/
		return true;
	}
	void OpenGLShader::Reflect(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binaries)
	{
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
		//std::cout << "Reflection took: " << timer.ElapsedMillis() << "ms" << std::endl;
	}
	void OpenGLShader::Reload_RT(std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		CreateProgram(binaries);
	}
	static void CheckShader(GLuint id, GLuint type)
	{
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
				Window::SendDebugMessage({ Severity::Error, "Shader linking failed", std::string(infoLog.data()) });
			}
			break;
		default:
			break;
		};
	}

	void OpenGLShader::CreateProgram(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binary)
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : binary)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(OpenGLUtils::ShaderStageToGLType(stage)));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
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
			Window::SendDebugMessage({ Severity::Error, "Shader linking failed", std::string(infoLog.data()) });
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
	void OpenGLShader::ReflectVulkan(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binaries)
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

				descriptorSet.AddWriteDescriptor(writeDescriptor);
			}
		}
		std::cout << "Reflection took: " << timer.ElapsedMillis() << "ms" << std::endl;
	}
}
#endif

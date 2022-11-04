
#include "OpenGLShader.h"
#ifdef HZR_INCLUDE_OPENGL

#include "../OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include "../Textures/OpenGLImage2D.h"
#include "../OpenGLContext.h"
#include "OpenGLBuffers.h"
#include "Profiling/Timer.h"

#include "Backend/Core/ShaderCompiler.h"
#include "Backend/Core/Window.h"

#include <glad/glad.h>
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::OpenGL
{
	OpenGLShader::OpenGLShader(const std::vector<ShaderStageCode>& shaderCode)
	{
		for (auto& code : shaderCode)
			m_ShaderCode[code.Stage] = Buffer::Copy(code.ShaderCode);

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

		m_ShaderData.Stages.clear();
		ReflectVulkan(m_ShaderCode);
		Reflect(m_ShaderCode);

		Ref<OpenGLShader> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->CreateProgram();
			});
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
		ShaderData data = ShaderCompiler::GetShaderResources(binaries);

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
		std::cout << fmt::format("Shader reflection took {0}", timer.ElapsedMillis()) << std::endl;
		//OpenGLShaderCompiler::PrintReflectionData(data);
	}
	void OpenGLShader::Reload_RT()
	{
		if (m_ID)
			glDeleteProgram(m_ID);
		CreateProgram();
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
					message = std::string(infoLog.data());
				Window::SendDebugMessage({ Severity::Error, "Shader linking failed", message });
			}
			break;
		default:
			break;
		};
	}

	void OpenGLShader::CreateProgram()
	{
		HZR_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : m_ShaderCode)
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
			int infologLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
			std::vector<GLchar> infoLog(infologLength);
			GLsizei charsWritten = 0;

			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog.data());
			std::string message;

			if (charsWritten)
				message = std::string(infoLog.data());
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
		m_ShaderData = ShaderCompiler::GetShaderResources(binaries);

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

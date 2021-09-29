#pragma once
#include "Hazard/Rendering/Shader.h"
#include "Hazard/Rendering/Buffers/Buffers.h"

#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLShader : public Shader 
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath);
		~OpenGLShader();

		void Reload() override;
		void Bind() override;
		void Unbind() override;
		void SetUniformBuffer(const std::string& name, void* data) override;

		std::unordered_map<ShaderType, ShaderStageData> GetShaderData() { return m_ShaderStageData; };

	private:

		bool CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void CompileOrGetOpenGLBinaries(bool forceRecompile = false);
		void CreateProgram();
		void Reflect(GLenum stage, std::vector<uint32_t> data);

		std::string m_FilePath;


		uint32_t m_ID;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
		std::unordered_map<ShaderType, ShaderStageData> m_ShaderStageData;
		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;

	};
}
#pragma once
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Pipeline/Buffers.h"

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
		UniformBuffer& GetUniform(const std::string name) override { return *m_UniformBuffers[name]; };

		const ShaderData& GetShaderData() { return m_ShaderData; };

	private:

		bool CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		void CompileOrGetOpenGLBinaries(bool forceRecompile = false);
		void CreateProgram();
		void Reflect();

		std::string m_FilePath;

		uint32_t m_ID;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<uint32_t, std::string> m_OpenGLSourceCode;
		ShaderData m_ShaderData;
		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
	};
}
#pragma once
#include "Hazard/Rendering/Shader.h"

#include "glad/glad.h"

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

	private:

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& sources);
		void CompileOrGetOpenGLBinaries();
		void Reflect(GLenum stage, std::vector<uint32_t> data);

		std::string m_FilePath;


		uint32_t m_ID;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;

	};
}
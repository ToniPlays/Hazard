#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		~OpenGLShader();

		void Reload() override;
		bool SetUniformBuffer(const std::string& name, void* data, uint32_t size) override;
		void Set(const std::string& name, uint32_t index, uint32_t value) override;
		void Set(const std::string& name, uint32_t index, Ref<Image2D> value) override;
		Ref<UniformBuffer> GetUniform(const std::string& name) override { return m_UniformBuffers[name]; };

		const ShaderData& GetShaderData() { return m_ShaderData; };

		//OpenGL specific
		void Reload_RT(std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries);
		uint32_t GetProgramID() const { return m_ID; }

	private:
		void CreateProgram(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binary);
		void Reflect(std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries);

		std::string m_FilePath;
		uint32_t m_ID = 0;

		ShaderData m_ShaderData;
		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
	};
}
#endif

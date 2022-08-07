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
		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Bind_RT(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind() override;
		bool SetUniformBuffer(const std::string& name, void* data, uint32_t size) override;
		void Set(const std::string& name, uint32_t index, uint32_t value) override;
		void Set(const std::string& name, uint32_t index, Ref<Image2D> value) override;
		UniformBuffer& GetUniform(const std::string& name) override { return *m_UniformBuffers[name]; };

		const ShaderData& GetShaderData() { return m_ShaderData; };

	private:

		bool CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources);
		bool CompileOrGetOpenGLBinaries(bool forceRecompile = false);
		void CreateProgram();
		void Reflect();

		std::string m_FilePath;
		uint32_t m_ID;

		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<uint32_t, std::string> m_OpenGLSourceCode;
		ShaderData m_ShaderData;
		std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
	};
}
#endif

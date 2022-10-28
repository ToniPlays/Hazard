#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"
#include "Backend/Core/Texture/Image2D.h"
#include "Backend/Core/Texture/Texture.h"

#include "OpenGLDescriptorSet.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		~OpenGLShader();

		void Reload() override;
		bool SetUniformBuffer(uint32_t set, uint32_t binding, void* data, uint32_t size) override;
		virtual void Set(const std::string& name, uint32_t index, Ref<Image2D> image) override;
		virtual void Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap) override;

		const ShaderData& GetShaderData() { return m_ShaderData; };

		//OpenGL specific
		void Reload_RT(std::unordered_map<ShaderStage, Buffer> binaries);
		uint32_t GetProgramID() const { return m_ID; }

		std::unordered_map<uint32_t, OpenGLDescriptorSet> GetDescriptorSets() { return m_DescriptorSet; };
		const OpenGLDescriptorSet& GetDescriptorSet(uint32_t index) { return m_DescriptorSet[index]; };

	private:
		void CreateProgram(const std::unordered_map<ShaderStage, Buffer>& binary);
		void ReflectVulkan(const std::unordered_map<ShaderStage, Buffer>& binaries);
		void Reflect(const std::unordered_map<ShaderStage, Buffer>& binaries);

		std::unordered_map<uint32_t, OpenGLDescriptorSet> m_DescriptorSet;

		std::string m_FilePath;
		uint32_t m_ID = 0;
		ShaderData m_ShaderData;
	};
}
#endif

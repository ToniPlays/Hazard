#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/Pipeline/Shader.h"
#include "Core/Pipeline/Buffers.h"

namespace HazardRenderer::Metal
{
    class MetalShader : public Shader
    {
    public:
        MetalShader(const std::string& filePath);
        MetalShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath);
        ~MetalShader();

        void Reload() override;
        void Bind(Ref<RenderCommandBuffer> cmdBufer) override;
        void Unbind() override;
        void SetUniformBuffer(const std::string& name, void* data, uint32_t size) override;
        void Set(const std::string& name, uint32_t index, uint32_t value) override;
        void Set(const std::string& name, uint32_t index, Ref<Texture2D>& value) override;
        UniformBuffer& GetUniform(const std::string& name) override { return *m_UniformBuffers[name]; };

        const ShaderData& GetShaderData() override { return m_ShaderData; };

    private:

        std::string m_FilePath;

        ShaderData m_ShaderData;
        std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
    };
}
#endif

#pragma once
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Pipeline/Buffers.h"

namespace Hazard::Rendering::Metal
{
    class MetalShader : public Shader
    {
    public:
        MetalShader(const std::string& filePath) {};
        MetalShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath) {};
        ~MetalShader() {};

        void Reload() override {};
        void Bind() override {};
        void Unbind() override {};
        void SetUniformBuffer(const std::string& name, void* data) override {};
        void Set(const std::string& name, uint32_t index, uint32_t value) override {};
        void Set(const std::string& name, uint32_t index, Ref<Texture2D>& value) override {};
        UniformBuffer& GetUniform(const std::string& name) override { return *m_UniformBuffers[name]; };

        const ShaderData& GetShaderData() override { return m_ShaderData; };

    private:

        std::string m_FilePath;

        ShaderData m_ShaderData;
        std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
    };
}

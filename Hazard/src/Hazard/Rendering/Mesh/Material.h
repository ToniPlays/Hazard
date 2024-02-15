#pragma once

#include "Hazard/Assets/Asset.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/DescriptorSet.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "UtilityCore.h"

#include "Core/Rendering/Shader.h"

namespace Hazard
{
    struct MaterialParam
    {
        std::string Name;
        HazardRenderer::ShaderDataType Type;
        uint32_t Offset;
    };

    class Material : public Asset
    {
    public:
        Material() = default;
        Material(Ref<HazardRenderer::Pipeline> pipeline);
        ~Material();

        AssetType GetType() const override { return AssetType::Material; }
        
        Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; };
        Ref<HazardRenderer::DescriptorSet> GetDescriptorSet() const { return m_DescriptorSet; };
        Buffer GetPushConstantData() const { return m_PushConstants; }

        void SetPipeline(Ref<HazardRenderer::Pipeline> pipeline);
        void Set(const std::string& name, Ref<HazardRenderer::Cubemap> cubemap);

        template<typename T>
        void Set(const std::string& name, T value) {
            if (!m_MaterialParams.contains(name)) return;

            auto& param = m_MaterialParams[name];
            m_PushConstants.Write(&value, ShaderDataTypeSize(param.Type), param.Offset);
        }

    private:
        void Invalidate();
        void InvalidatePushConstants();

    private:
        Ref<HazardRenderer::Pipeline> m_Pipeline;
        Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
        std::unordered_map<std::string, MaterialParam> m_MaterialParams;

        Buffer m_PushConstants;
    };
}

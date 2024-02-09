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
        HazardRenderer::ShaderDataType Type;
        uint32_t Offset;
    };

    class Material : public Asset
    {
    public:
        
        Material();
        Material(Ref<HazardRenderer::Pipeline> pipeline, HazardRenderer::DescriptorSetLayout layout = HazardRenderer::DescriptorSetLayout());

        AssetType GetType() const override { return AssetType::Material; }
        
        Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; };
        Ref<HazardRenderer::DescriptorSet> GetDescriptorSet() const { return m_DescriptorSet; };
        void SetPipeline(Ref<HazardRenderer::Pipeline> pipeline) { m_Pipeline = pipeline; };
        
    private:
        Ref<HazardRenderer::Pipeline> m_Pipeline;
        Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
    };
}

#pragma once

#include "Hazard/Assets/Asset.h"
#include "Backend/Core/Pipeline.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "UtilityCore.h"

#include "Backend/Core/Shader.h"

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
        Material(AssetHandle pipelineHandle);
        
        template<typename T>
        T Get(const std::string& key)
        {
            return T();
        };
    
        void* GetValue(const std::string& key)
        {
            return nullptr;
        };
        
        template<typename T>
        void Set(const std::string& key, T& value)
        {

        }
        
        
        AssetHandle GetPipeline() const { return m_PipelineHandle; };
        Ref<HazardRenderer::DescriptorSet> GetDescriptorSet() const { return m_DescriptorSet; };
        void SetPipeline(AssetHandle handle) { m_PipelineHandle = handle; };
        
    private:
        AssetHandle m_PipelineHandle;
        Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
    };
}

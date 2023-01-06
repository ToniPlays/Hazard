#pragma once

#include "Hazard/Assets/Asset.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "UtilityCore.h"

#include "Backend/Core/Pipeline/Shader.h"

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
        Material(Ref<HazardRenderer::Pipeline> pipeline)
        {
            SetPipeline(pipeline);
        }
        
        template<typename T>
        T Get(const std::string& key)
        {
            uint32_t offset = m_Params[key].Offset;
            return m_ParameterBuffer.Read<T>(offset);
        };
    
        void* GetValue(const std::string& key)
        {
            auto& param = m_Params[key];
            return m_ParameterBuffer.Read(ShaderDataTypeSize(param.Type), param.Offset);
        };
        
        template<typename T>
        void Set(const std::string& key, T& value)
        {
            uint32_t offset = m_Params[key].Offset;
            return m_ParameterBuffer.Write(&value, sizeof(T), offset);
        }
        
        void SetTexture(const std::string& name, Ref<Texture2DAsset> texture)
        {
            m_Textures[name] = texture;
        }
        
        const std::unordered_map<std::string, MaterialParam>& GetParameters() const { return m_Params; }
        const std::unordered_map<std::string, Ref<Texture2DAsset>>& GetTextures() const { return m_Textures; }
        Buffer GetBuffer() const { return m_ParameterBuffer; }
        
        Ref<HazardRenderer::Pipeline> GetPipeline() const { return m_Pipeline; };
        void SetPipeline(Ref<HazardRenderer::Pipeline> pipeline);
        
    private:
        
        void UpdateParameters(const HazardRenderer::ShaderPushConstantRange& constant);
        
    private:
        Ref<HazardRenderer::Pipeline> m_Pipeline;
        Buffer m_ParameterBuffer;
        std::unordered_map<std::string, MaterialParam> m_Params;
        std::unordered_map<std::string, Ref<Texture2DAsset>> m_Textures;
        
    };
}

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
        Material(AssetHandle pipelineHandle)
        {
            SetPipeline(pipelineHandle);
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
        
        void SetTexture(const std::string& name, AssetHandle handle)
        {
            m_Textures[name] = handle;
        }
        
        const std::unordered_map<std::string, MaterialParam>& GetParameters() const { return m_Params; }
        const std::unordered_map<std::string, AssetHandle>& GetTextures() const { return m_Textures; }
        Buffer GetBuffer() const { return m_ParameterBuffer; }
        
        AssetHandle GetPipeline() const { return m_PipelineHandle; };
        void SetPipeline(AssetHandle handle) { m_PipelineHandle = handle; };
        
    private:
        
        void UpdateParameters(const HazardRenderer::ShaderPushConstantRange& constant);
        
    private:
        AssetHandle m_PipelineHandle;
        Buffer m_ParameterBuffer;
        std::unordered_map<std::string, MaterialParam> m_Params;
        std::unordered_map<std::string, AssetHandle> m_Textures;
    };
}

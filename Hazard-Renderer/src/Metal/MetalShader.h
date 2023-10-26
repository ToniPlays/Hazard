#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/Rendering/Shader.h"
#include "MetalDescriptorSet.h"
#include "MetalGPUBuffer.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    struct MetalInputBuffer
    {
        Ref<MetalGPUBuffer> Buffer;
        size_t Offset = 0;
    };

    class MetalShader : public Shader
    {
    public:
      
        MetalShader(const std::vector<ShaderStageCode>& shaderCode);
        ~MetalShader();
        
        void Reload() override;

        const ShaderData& GetShaderData() const override { return m_ShaderData; };
        std::unordered_map<uint32_t, Buffer> GetShaderCode() const override { return m_ShaderCode; };
        
        //Metal specific
        MTL::Function* GetFunction(uint32_t stage) { return m_Functions[stage]; }
        
    private:
        void Reflect();
        
    private:
        ShaderData m_ShaderData;
        std::unordered_map<uint32_t, Buffer> m_ShaderCode;
        std::unordered_map<uint32_t, MTL::Function*> m_Functions;
        
        std::unordered_map<uint32_t, MetalDescriptorSet> m_DescriptorSet;
        std::unordered_map<uint32_t, MetalInputBuffer> m_InputBuffers;
    };
}
#endif

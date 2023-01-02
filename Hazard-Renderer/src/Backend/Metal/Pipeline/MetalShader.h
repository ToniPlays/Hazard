#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"
#include "MetalDescriptorSet.h"
#include "MetalVertexBuffer.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    struct MetalInputBuffer
    {
        Ref<MetalVertexBuffer> Buffer;
        size_t Offset = 0;
    };

    class MetalShader : public Shader
    {
    public:
      
        MetalShader(const std::vector<ShaderStageCode>& shaderCode);
        ~MetalShader();
        
        void Reload() override;
        void Set(const std::string& name, uint32_t index, Ref<Image2D> image) override;
        void Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap) override;
        void Set(const std::string& name, uint32_t index, Ref<AccelerationStructure> accelerationStructure) override;
        void Set(const std::string& name, uint32_t index, Ref<BufferBase> buffer) override;
        void Set(const std::string& name, Buffer buffer) override;
        
        void Set(uint32_t index, Ref<VertexBuffer> buffer, size_t offset = 0) override;

        const ShaderData& GetShaderData() override { return m_ShaderData; };
        std::unordered_map<ShaderStage, Buffer> GetShaderCode() const override { return m_ShaderCode; };
        
        //Metal specific
        MTL::Function* GetFunction(const ShaderStage& stage) { return m_Functions[stage]; }
        void BindResources(MTL::RenderCommandEncoder* encoder);
        void BindResources(MTL::ComputeCommandEncoder* encoder);
        
    private:
        void Reflect();
        
    private:
        ShaderData m_ShaderData;
        std::unordered_map<ShaderStage, Buffer> m_ShaderCode;
        std::unordered_map<ShaderStage, MTL::Function*> m_Functions;
        
        std::unordered_map<uint32_t, MetalDescriptorSet> m_DescriptorSet;
        std::unordered_map<uint32_t, MetalInputBuffer> m_InputBuffers;
    };
}
#endif

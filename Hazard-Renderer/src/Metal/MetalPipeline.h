#pragma once

#include "Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Core/Rendering/Pipeline.h"
#include "MetalShader.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalPipeline : public Pipeline
    {
    public:
        MetalPipeline(PipelineSpecification* specs);
        ~MetalPipeline();

        PipelineSpecification GetSpecifications() override { return m_Specs; }
        const PipelineSpecification GetSpecifications() const override { return m_Specs; }
        Ref<Shader> GetShader() override { return m_Shader; }
        void SetRenderPass(Ref<RenderPass> renderPass) override;
        bool IsValid() const override { return m_Pipeline; }

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
    
        void Invalidate() override;
        void Invalidate_RT();
        
        //Metal specific
        MTL::PrimitiveType GetMetalPrimitiveType() const { return m_PrimitiveType; }
        MTL::RenderPipelineState* GetMetalRenderPipelineState() const { return m_Pipeline; }
        
        void BindGraphics(MTL::RenderCommandEncoder* encoder);
        void BindCompute(MTL::ComputeCommandEncoder* encoder);
        
    private:
        void InvalidateGraphicsPipeline();
        void InvalidateComputePipeline();
    private:
        PipelineSpecification m_Specs;
        Ref<MetalShader> m_Shader;
        BufferLayout m_Layout;
        
        MTL::PrimitiveType m_PrimitiveType;
        
        MTL::RenderPipelineDescriptor* m_PipelineDescriptor;
        MTL::ComputePipelineDescriptor* m_ComputeDescriptor;
        
        MTL::RenderPipelineState* m_Pipeline = nullptr;
        MTL::ComputePipelineState* m_ComputePipeline = nullptr;
        
        MTL::DepthStencilState* m_DepthState = nullptr;
        
    };
}
#endif

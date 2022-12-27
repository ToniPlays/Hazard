
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"
#include "MetalFrameBuffer.h"
#include "MTLUtils.h"
#include "Renderer.h"


namespace HazardRenderer::Metal
{
    
    MetalPipeline::MetalPipeline(PipelineSpecification* specs) : m_Specs(*specs)
    {
        m_PrimitiveType = DrawTypeToMTLPrimitive(m_Specs.DrawType);
        m_PipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
        SetDebugLabel(m_PipelineDescriptor, m_Specs.DebugName);
        
        if(specs->pBufferLayout)
            m_Layout = BufferLayout(*specs->pBufferLayout);
        
        std::vector<ShaderStageCode> code(specs->ShaderCodeCount);
        for (uint32_t i = 0; i < specs->ShaderCodeCount; i++)
            code[i] = specs->pShaderCode[i];

        m_Shader = Shader::Create(code).As<MetalShader>();
        
        Invalidate();
    }
    MetalPipeline::~MetalPipeline()
    {
        m_PipelineDescriptor->release();
        m_Pipeline->release();
    }
    void MetalPipeline::SetRenderPass(Ref<RenderPass> renderPass)
    {
        if(m_Specs.pTargetRenderPass == renderPass) return;
        m_Specs.pTargetRenderPass = renderPass;
        
        Invalidate();
    }

    void MetalPipeline::Invalidate()
    {
        Ref<MetalPipeline> instance = this;
        
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
        });
    }
    void MetalPipeline::Invalidate_RT()
    {
        if(m_Specs.Usage == PipelineUsage::GraphicsBit)
            InvalidateGraphicsPipeline();
        else if(m_Specs.Usage == PipelineUsage::ComputeBit)
            InvalidateComputePipeline();
    }
    void MetalPipeline::InvalidateGraphicsPipeline()
    {
        if(!m_Specs.pTargetRenderPass) return;
        
        if(m_Pipeline)
            m_Pipeline->release();
        
        auto device = MetalContext::GetMetalDevice();
        auto fb = m_Specs.pTargetRenderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();

        //Create vertex input descriptor    
        MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
        
        uint32_t attribIndex = 0;
        if(m_Layout.GetBufferStride(PerVertex))
        {
            auto layout = vertexDescriptor->layouts()->object(28);
            layout->setStride(m_Layout.GetBufferStride(PerVertex));
            layout->setStepRate(MTL::StepFunctionPerVertex);

            for(auto& element : m_Layout.GetElements())
            {
                if(element.ElementDivisor == PerInstance) continue;
                
                auto object = vertexDescriptor->attributes()->object(attribIndex);
                object->setBufferIndex(28);
                object->setOffset(element.Offset);
                object->setFormat(ShaderDataTypeToMTLVertexFormat(element.Type));
                attribIndex++;
            }
            
        }
        if(m_Layout.GetBufferStride(PerInstance))
        {
            auto layout = vertexDescriptor->layouts()->object(29);
            layout->setStepRate(MTL::StepFunctionPerInstance);
            layout->setStride(m_Layout.GetBufferStride(PerInstance));
            
            for(auto& element : m_Layout.GetElements())
            {
                if(element.ElementDivisor == PerVertex) continue;
                
                auto object = vertexDescriptor->attributes()->object(attribIndex);
                object->setBufferIndex(PerInstance);
                object->setOffset(element.Offset);
                object->setFormat(ShaderDataTypeToMTLVertexFormat(element.Type));
                attribIndex++;
            }
        }
        
        m_PipelineDescriptor->setVertexFunction(m_Shader->GetFunction(ShaderStage::Vertex));
        m_PipelineDescriptor->setFragmentFunction(m_Shader->GetFunction(ShaderStage::Fragment));
        m_PipelineDescriptor->setInputPrimitiveTopology(DrawTypeToMTLTopology(m_Specs.DrawType));
        m_PipelineDescriptor->setVertexDescriptor(vertexDescriptor);
        
        size_t attachmentCount = fb->GetSpecification().SwapChainTarget ? 1 : fb->GetColorAttachmentCount();
        
        if(fb->GetSpecification().SwapChainTarget)
        {
            auto attachment = m_PipelineDescriptor->colorAttachments()->object(0);
            attachment->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
            
            attachment->setBlendingEnabled(true);
            attachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
            attachment->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
            attachment->setRgbBlendOperation(MTL::BlendOperationAdd);
            attachment->setAlphaBlendOperation(MTL::BlendOperationAdd);
            attachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
            attachment->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
        }
        else
        {
            //Set color attachments
            for(uint32_t i = 0; i < attachmentCount; i++)
            {
                auto colorAttachment = fb->GetImage(i).As<MetalImage2D>();
                auto obj = m_PipelineDescriptor->colorAttachments()->object(i);
                
                obj->setPixelFormat(ImageFormatToMTLFormat(colorAttachment->GetFormat()));
                
                obj->setBlendingEnabled(true);
                obj->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
                obj->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
                obj->setRgbBlendOperation(MTL::BlendOperationAdd);
                obj->setAlphaBlendOperation(MTL::BlendOperationAdd);
                obj->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
                obj->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
            }
            
            if(fb->GetDepthImage())
            {
                auto depthAttachment = fb->GetDepthImage();
                m_PipelineDescriptor->setDepthAttachmentPixelFormat(ImageFormatToMTLFormat(depthAttachment->GetFormat()));
                
            }
        }
        
        NS::Error* error;
        
        m_Pipeline = device->GetMetalDevice()->newRenderPipelineState(m_PipelineDescriptor, &error);
        
        if(error)
            std::cout << error->description()->utf8String() << std::endl;
        
        //Create depth stencil
        
        if(fb->GetDepthImage() && !fb->GetSpecification().SwapChainTarget)
        {
            MTL::DepthStencilDescriptor* depth = MTL::DepthStencilDescriptor::alloc()->init();
            depth->setDepthWriteEnabled(m_Specs.DepthWrite);
            depth->setDepthCompareFunction(MTL::CompareFunctionLess);
            
            m_DepthState = device->GetMetalDevice()->newDepthStencilState(depth);
        }
    }

    void MetalPipeline::InvalidateComputePipeline()
    {
        
    }
    void MetalPipeline::BindGraphics(MTL::RenderCommandEncoder* encoder)
    {
        encoder->setRenderPipelineState(m_Pipeline);
        if(m_DepthState)
            encoder->setDepthStencilState(m_DepthState);
        m_Shader->BindResources(encoder);
    }
    void MetalPipeline::BindCompute(MTL::ComputeCommandEncoder* encoder)
    {
        encoder->setComputePipelineState(m_ComputePipeline);
        m_Shader->BindResources(encoder);
    }
}
#endif


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
    void MetalPipeline::Invalidate()
    {
        Ref<MetalPipeline> instance = this;
        
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
        });
    }
    void MetalPipeline::Invalidate_RT()
    {
        if(m_Pipeline)
            m_Pipeline->release();
        
        
        auto device = MetalContext::GetMetalDevice();
        auto fb = m_Specs.pTargetRenderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();

        //Create vertex input descriptor    
        MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
        
        uint32_t attribIndex = 0;
        if(m_Layout.GetBufferStride(PerVertex))
        {
            vertexDescriptor->layouts()->object(0)->setStepRate(MTL::StepFunctionPerVertex);
            vertexDescriptor->layouts()->object(0)->setStride(m_Layout.GetBufferStride(PerVertex));

            for(auto& element : m_Layout.GetElements())
            {
                if(element.ElementDivisor == PerInstance) continue;
                
                auto object = vertexDescriptor->attributes()->object(attribIndex);
                object->init();
                object->setBufferIndex(PerVertex);
                object->setOffset(element.Offset);
                object->setFormat(ShaderDataTypeToMTLVertexFormat(element.Type));
                attribIndex++;
            }
            
        }
        if(m_Layout.GetBufferStride(PerInstance))
        {
            vertexDescriptor->layouts()->object(1)->setStepRate(MTL::StepFunctionPerInstance);
            vertexDescriptor->layouts()->object(1)->setStride(m_Layout.GetBufferStride(PerInstance));
            
            for(auto& element : m_Layout.GetElements())
            {
                if(element.ElementDivisor == PerVertex) continue;
                
                auto object = vertexDescriptor->attributes()->object(attribIndex);
                object->init();
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
        }
        else
        {
            //Set color attachments
            for(uint32_t i = 0; i < attachmentCount; i++)
            {
                auto colorAttachment = fb->GetImage(i).As<MetalImage2D>();
                auto obj = m_PipelineDescriptor->colorAttachments()->object(i);
                obj->init();
                obj->setPixelFormat(ImageFormatToMTLFormat(colorAttachment->GetFormat()));
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
        
    }
}
#endif

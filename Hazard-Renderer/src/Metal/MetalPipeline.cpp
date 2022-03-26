
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"

#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
        m_Descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
        m_Shader = Shader::Create(specs->ShaderPath).As<MetalShader>();
        Invalidate();
    }
    MetalPipeline::~MetalPipeline()
    {
        
    }
    void MetalPipeline::Invalidate()
    {
        MTL::Device* device = MetalContext::GetMetalDevice();
        m_Descriptor->setVertexFunction(m_Shader->GetFunction(ShaderType::Vertex));
        m_Descriptor->setFragmentFunction(m_Shader->GetFunction(ShaderType::Fragment));
        m_Descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        m_Descriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);
        
        
        MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::vertexDescriptor();
        vertexDescriptor->layouts()->object(0)->setStride(3 * sizeof(float));
        
        vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
        vertexDescriptor->attributes()->object(0)->setOffset(0);
        vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
        vertexDescriptor->attributes()->object(1)->setBufferIndex(0);
        vertexDescriptor->attributes()->object(1)->setOffset(3 * sizeof(float));
        vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat4);
        
        m_Descriptor->setVertexDescriptor(vertexDescriptor);
        
        NS::Error* error;
        m_Pipeline = device->newRenderPipelineState(m_Descriptor, &error);
        if(error)
            std::cout << error->description()->utf8String() << std::endl;
        
        
        
    }
    void MetalPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
    {
        MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        encoder->setRenderPipelineState(m_Pipeline);
        encoder = nullptr;
    }
    void MetalPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)count);
        encoder = nullptr;
    }
    void MetalPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)count);
        encoder = nullptr;
    }
}
#endif

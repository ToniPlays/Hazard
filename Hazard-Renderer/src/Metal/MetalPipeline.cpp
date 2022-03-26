
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"

#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
    }
    MetalPipeline::~MetalPipeline()
    {
        
    }
    void MetalPipeline::Invalidate() {
        
    }
    void MetalPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
    {
        //MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        //encoder->setRenderPipelineState(m_Pipeline);
    }
    void MetalPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        //MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        //encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)count);
    }
    void MetalPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        
    }
}
#endif


#include <hzrpch.h>
#include "MetalPipeline.h"
#include "MetalRenderCommandBuffer.h"

namespace Hazard::Rendering::Metal {
    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
        m_Shader = Shader::Create(specs->ShaderPath);
    }
    MetalPipeline::~MetalPipeline()
    {
        
    }
    void MetalPipeline::Invalidate() {
        
    }
    void MetalPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
    {
        MTL::CommandBuffer* cmdBuffer = commandBuffer.As<MetalRenderCommandBuffer>()->GetMetalCommandBuffer();
    }
    void MetalPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        
    }
    void MetalPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        
    }
}

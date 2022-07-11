#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal
{
    class MetalPipeline : public Pipeline
    {
    public:
        MetalPipeline(PipelineSpecification* specs);
        virtual ~MetalPipeline();

        PipelineSpecification GetSpecifications() override { return m_Specs; }
        const PipelineSpecification GetSpecifications() const override { return m_Specs; }
        Ref<Shader> GetShader() override { return m_Shader; }
        void SetRenderPass(Ref<RenderPass> renderPass) override {}

        void Invalidate() override;
        void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
        void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;
        void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;
        void DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount) override;

    private:
        PipelineSpecification m_Specs;
        Ref<Shader> m_Shader;
        
        MTL::RenderPipelineState* m_Pipeline = nullptr;
    };
}
#endif

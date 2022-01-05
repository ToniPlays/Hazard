#pragma once

#include "Hazard/Rendering/Pipeline/Pipeline.h"
#include "Hazard/Rendering/Pipeline/Shader.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace Hazard::Rendering::Metal
{
    class MetalPipeline : public Pipeline
    {
    public:
        MetalPipeline(PipelineSpecification* specs);
        virtual ~MetalPipeline();

        PipelineSpecification GetSpecifications() override { return m_Specs; }
        const PipelineSpecification GetSpecifications() const override { return m_Specs; }
        Ref<Shader> GetShader() override { return m_Shader; }

        void Invalidate() override;
        void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
        void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;
        void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;

    private:
        PipelineSpecification m_Specs;
        Ref<Shader> m_Shader;
        uint32_t m_DrawType, m_PolygonMode;
        
        MTL::RenderPipelineState* m_Pipeline;
    };
}

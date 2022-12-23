#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

namespace HazardRenderer::Metal
{
    class MetalPipeline : public Pipeline
    {
    public:
        MetalPipeline(PipelineSpecification* specs) {};
        ~MetalPipeline() {};

        PipelineSpecification GetSpecifications() override { return m_Specs; }
        const PipelineSpecification GetSpecifications() const override { return m_Specs; }
        Ref<Shader> GetShader() override { return m_Shader; }
        void SetRenderPass(Ref<RenderPass> renderPass) override {};

        bool IsCompatibleWith(Ref<Shader> shader) const override { return false; };
        bool IsValid() const override { return false; }

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

        void Invalidate() override {};
        void Invalidate_RT() {};
    private:
        PipelineSpecification m_Specs;
        Ref<Shader> m_Shader;
        BufferLayout m_Layout;
        
    };
}
#endif

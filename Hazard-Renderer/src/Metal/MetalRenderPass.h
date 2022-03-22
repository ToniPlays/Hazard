#pragma once

#include "Hazard/RenderContext/Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/Pipeline/RenderPass.h"


namespace Hazard::Rendering::Metal {
    class MetalRenderPass : public RenderPass {
    public:
        MetalRenderPass(RenderPassCreateInfo* info);
        ~MetalRenderPass();

        virtual RenderPassSpecification GetSpecs() { return m_Specs; };
        virtual const RenderPassSpecification GetSpecs() const { return m_Specs; };

    private:
        RenderPassSpecification m_Specs;
    };
}

#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/FrameBuffer.h"
#include "Core/Pipeline/RenderPass.h"


namespace HazardRenderer::Metal {
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
#endif

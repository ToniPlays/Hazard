#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/FrameBuffer.h"

namespace MTL {
class RenderPassDescriptor;
}

namespace HazardRenderer::Metal
{
    class MetalFrameBuffer : public FrameBuffer {
    public:
        MetalFrameBuffer(FrameBufferCreateInfo* info);
        ~MetalFrameBuffer();

    private:
    };
}
#endif

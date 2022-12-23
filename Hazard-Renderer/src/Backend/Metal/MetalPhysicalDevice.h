#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace MTL
{
    class Device;
    class CommandQueue;
}

namespace HazardRenderer::Metal {

    class MetalPhysicalDevice : public PhysicalDevice {
    public:
        MetalPhysicalDevice();
        
    private:
        
    };
}
#endif

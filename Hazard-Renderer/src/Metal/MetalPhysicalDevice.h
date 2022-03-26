#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_METAL
#include "Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace MTL {
    class Device;
}

namespace HazardRenderer::Metal {

    class MetalPhysicalDevice : public PhysicalDevice {
    public:
        MetalPhysicalDevice();

        virtual ~MetalPhysicalDevice() {};
        virtual std::string GetDeviceName() { return m_DeviceName; }
        virtual const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };
        
        
        MTL::Device* GetMetalDevice() { return m_Device; }
        static Ref<PhysicalDevice> Create();

    private:
        std::string m_DeviceName;
        PhysicalDeviceLimits m_Limits;
        
        MTL::Device* m_Device;
    };
}
#endif

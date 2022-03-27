#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL
#include "Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace MTL {
    class Device;
    class CommandQueue;
}

namespace HazardRenderer::Metal {

    class MetalPhysicalDevice : public PhysicalDevice {
    public:
        MetalPhysicalDevice();

        virtual ~MetalPhysicalDevice() {};
        virtual std::string GetDeviceName() { return m_DeviceName; }
        virtual const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };
        
        
        MTL::Device* GetMetalDevice() { return m_Device; }
        MTL::CommandQueue* GetMetalCommandQueue() { return m_Queue; }
        static Ref<PhysicalDevice> Create();

    private:
        std::string m_DeviceName;
        PhysicalDeviceLimits m_Limits;
        
        MTL::Device* m_Device;
        MTL::CommandQueue* m_Queue;
    };
}
#endif

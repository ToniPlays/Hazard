#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/GraphicsContext.h"
#include <GLFW/glfw3.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal {

    class MetalPhysicalDevice : public PhysicalDevice {
    public:
        MetalPhysicalDevice();
        
        std::string GetDeviceName() { return m_DeviceName; };
        const PhysicalDeviceLimits& GetDeviceLimits() const { return m_Limits; };
        const PhysicalDeviceCapabilities& GetDeviceCababilities() const { return m_Capabilities; };

        bool SupportsRaytracing() const { return false; };
        
        //Metal specific
        MTL::Device* GetMetalDevice() const { return m_Device; }
        
    private:
        std::string m_DeviceName;
        PhysicalDeviceLimits m_Limits;
        PhysicalDeviceCapabilities m_Capabilities;
        
        MTL::Device* m_Device;
        MTL::CommandQueue* m_GraphicsQueue;
    };
}
#endif

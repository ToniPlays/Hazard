
#include "MetalPhysicalDevice.h"

#ifdef HZR_INCLUDE_METAL

#import <Metal/Metal.h>

namespace HazardRenderer::Metal {

    MetalPhysicalDevice::MetalPhysicalDevice()
    {
        m_Device = MTL::CreateSystemDefaultDevice();
        m_Queue = m_Device->newCommandQueue();
        
        m_DeviceName = m_Device->name()->utf8String();
    }
}

#endif

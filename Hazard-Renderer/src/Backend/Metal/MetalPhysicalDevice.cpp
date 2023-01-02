
#include "MetalPhysicalDevice.h"

#ifdef HZR_INCLUDE_METAL

namespace HazardRenderer::Metal
{

    MetalPhysicalDevice::MetalPhysicalDevice()
    {
        m_Device = MTL::CreateSystemDefaultDevice();
        m_GraphicsQueue = m_Device->newCommandQueue();
        m_DeviceName = m_Device->name()->utf8String();
        
#ifdef HZR_PLATFORM_MACOS
        m_Capabilities.Discrete = m_Device->removable();
        m_Capabilities.LowPower = m_Device->lowPower();
#endif
        m_Capabilities.Swapchain = true;
        m_Capabilities.DiagnosticCheckpoint = false;
        m_Capabilities.DiagnosticConfig = false;
        m_Capabilities.AccelerationStructures = m_Device->supportsRaytracing();
        m_Capabilities.RayTracingPipeline = m_Device->supportsRaytracing();
        m_Capabilities.RayQuery = m_Device->supportsRaytracing();
        
        m_Limits.MaxTextureUnits = 32;
        m_Limits.MaxRecursionDepth = 1;
    }
}

#endif

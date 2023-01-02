#include "MetalWindowLayer.h"
#ifdef HZR_INCLUDE_METAL

#ifdef HZR_PLATFORM_IOS

    #import <Metal/Metal.h>
    #import <QuartzCore/CAMetalLayer.h>
    #include <QuartzCore/QuartzCore.h>

    namespace HazardRenderer::Metal
    {
        MetalWindowLayer::MetalWindowLayer(Window& window, MTL::Device* device)
        {
            
        }
        void MetalWindowLayer::Resize(uint32_t width, uint32_t height)
        {
            
        }

        CA::MetalDrawable* MetalWindowLayer::GetNextDrawable()
        {
            return nullptr;
        }
    }
    #endif
#endif

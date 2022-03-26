#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_METAL

struct GLFWwindow;

namespace MTL
{
    class Device;
    class CommandBuffer;
    class Drawable;
    class Texture;
}
namespace CA
{
    class MetalLayer;
    class MetalDrawable;
}

namespace HazardRenderer::Metal
{
    class MetalWindowLayer {
    public:
        MetalWindowLayer(GLFWwindow* window, MTL::Device* device);
        ~MetalWindowLayer() = default;
        
        void Resize(uint32_t width, uint32_t height);
        
        CA::MetalDrawable* GetNextDrawable();

    private:
        CA::MetalLayer* m_Layer;
    };
}
#endif

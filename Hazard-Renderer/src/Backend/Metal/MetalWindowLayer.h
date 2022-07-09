#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Window.h"

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
        MetalWindowLayer(Window& window, MTL::Device* device);
        ~MetalWindowLayer() = default;
        
        void Resize(uint32_t width, uint32_t height);
        uint32_t GetWidth() { return m_Width; }
        uint32_t GetHeight() { return m_Height; }
        
        CA::MetalDrawable* GetNextDrawable();
        MTL::Texture* GetDrawableTexture();

    private:
        CA::MetalLayer* m_Layer;
        uint32_t m_Width;
        uint32_t m_Height;
    };
}
#endif

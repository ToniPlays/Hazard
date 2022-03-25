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

namespace HazardRenderer::Metal
{
    class MetalLayer {
    public:
        MetalLayer(GLFWwindow* window);
        ~MetalLayer() = default;
        MTL::Device* GetDevice();
        void Present(MTL::CommandBuffer* buffer);
        MTL::Drawable* GetDrawable();
        MTL::Texture* GetDrawableTexture();

    private:
    
    };
}
#endif
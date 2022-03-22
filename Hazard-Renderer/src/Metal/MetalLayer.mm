#include <hzrpch.h>
#include "MetalLayer.h"
#include "MetalContext.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.hpp>

namespace Hazard::Rendering::Metal
{
    CAMetalLayer* layer;
    id<MTLDevice> dev;

    MetalLayer::MetalLayer(GLFWwindow* window)
    {
        NSWindow* nswin = glfwGetCocoaWindow(window);
        layer = [CAMetalLayer layer];
        
        dev = MTLCreateSystemDefaultDevice();
        
        layer.device = dev;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        layer.frame.size = { 1920,  1080 };
        nswin.contentView.layer = layer;
        nswin.contentView.wantsLayer = YES;
    }
    MTL::Device* MetalLayer::GetDevice() {
        return (__bridge MTL::Device*)dev;
    }
    void MetalLayer::Present(MTL::CommandBuffer* buffer)
    {
        buffer->presentDrawable(GetDrawable());
    }
    MTL::Drawable* MetalLayer::GetDrawable()
    {
        return (__bridge MTL::Drawable*)layer.nextDrawable;
    }
    MTL::Texture* MetalLayer::GetDrawableTexture() {
        return (__bridge MTL::Texture*)layer.nextDrawable.texture;
    }
}


#include "MetalWindowLayer.h"
#include "MetalContext.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <OpenGL/OpenGL.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.h>

namespace HazardRenderer::Metal
{
    
    MetalWindowLayer::MetalWindowLayer(GLFWwindow* window, MTL::Device* device)
    {
        int w, h;
        
        glfwGetWindowSize(window, &w, &h);
        
        id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
        
        NSWindow* nswin = glfwGetCocoaWindow(window);
        CAMetalLayer* layer = [CAMetalLayer layer];
        layer.device = dev;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        layer.frame.size = { (CGFloat)w, (CGFloat)h };
        
        nswin.contentView.layer = layer;
        nswin.contentView.wantsLayer = YES;
        
        m_Layer = (__bridge CA::MetalLayer*)layer;
         
    }
    CA::MetalDrawable* MetalWindowLayer::GetNextDrawable()
    {
        CAMetalLayer* metalLayer = (__bridge CAMetalLayer*)m_Layer;
        id<CAMetalDrawable> metalDrawable      = [metalLayer nextDrawable];
        return (__bridge CA::MetalDrawable* ) metalDrawable;
    }
    MTL::Texture* MetalWindowLayer::GetDrawableTexture() {
        return nullptr;
    }
}


#include "MetalWindowLayer.h"
#ifdef HZR_INCLUDE_METAL
    #ifdef HZR_PLATFORM_MACOS

    #include "MetalContext.h"

    #define GLFW_EXPOSE_NATIVE_COCOA

    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>

    #import <Metal/Metal.h>
    #import <QuartzCore/CAMetalLayer.h>
    #include <QuartzCore/QuartzCore.h>

    namespace HazardRenderer::Metal
    {
        MetalWindowLayer::MetalWindowLayer(Window& window, MTL::Device* device)
        {
            int w, h;
            w = window.GetWidth();
            h = window.GetHeight();
            
            id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
            
            NSWindow* nswin = glfwGetCocoaWindow((GLFWwindow*)window.GetNativeWindow());
            CAMetalLayer* layer = [CAMetalLayer layer];
            layer.device = dev;
            layer.drawableSize = { (CGFloat)w, (CGFloat)h };
            layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
            layer.frame.size = { (CGFloat)w, (CGFloat)h };
            
            nswin.contentView.layer = layer;
            nswin.contentView.wantsLayer = YES;
            
            m_Width = w;
            m_Height = h;
            
            m_Layer = (__bridge CA::MetalLayer*)layer;
             
        }
        void MetalWindowLayer::Resize(uint32_t width, uint32_t height)
        {
            CAMetalLayer* layer = (__bridge CAMetalLayer*)m_Layer;
            layer.frame.size = { (CGFloat)width, (CGFloat)height} ;
            layer.drawableSize = { (CGFloat)width, (CGFloat)height };
            
            m_Width = layer.drawableSize.width;
            m_Height = layer.drawableSize.height;
        }

        CA::MetalDrawable* MetalWindowLayer::GetNextDrawable()
        {
            CAMetalLayer* metalLayer = (__bridge CAMetalLayer*)m_Layer;
            id<CAMetalDrawable> metalDrawable      = [metalLayer nextDrawable];
            return (__bridge CA::MetalDrawable* ) metalDrawable;
        }
    }
    #endif
#endif

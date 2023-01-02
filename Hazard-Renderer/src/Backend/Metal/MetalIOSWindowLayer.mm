#include "MetalWindowLayer.h"
#ifdef HZR_INCLUDE_METAL

#ifdef HZR_PLATFORM_IOS

    #import <Metal/Metal.h>
    #import <QuartzCore/CAMetalLayer.h>
    #import <CoreGraphics/CGColor.h>
    #import <Foundation/Foundation.h>
    #import <UIKit/UIKit.h>

    namespace HazardRenderer::Metal
    {
        MetalWindowLayer::MetalWindowLayer(Window& window, MTL::Device* device)
        {
            int w, h;
            w = window.GetWidth();
            h = window.GetHeight();
            
            CGColorRef color = CGColorCreateSRGB(1.0f, 1.0f, 1.0f, 1.0f);
            
            UIApplicationMain(0, nil, nil, [NSString stringWithUTF8String: "MTKViewController"]);
            
            id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
            
            CAMetalLayer* metalLayer = [CAMetalLayer layer];
            metalLayer.device = dev;
            metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
            metalLayer.drawableSize = { (CGFloat)w, (CGFloat)h };
            metalLayer.frame.size = { (CGFloat)w, (CGFloat)h };
            metalLayer.backgroundColor = color;
                        
            m_Width = w;
            m_Height = h;
            
            m_Layer = (__bridge CA::MetalLayer*)metalLayer;
        }
        void MetalWindowLayer::Resize(uint32_t width, uint32_t height)
        {
            
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

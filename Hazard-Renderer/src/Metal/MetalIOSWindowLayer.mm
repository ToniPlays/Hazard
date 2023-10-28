#include "MetalWindowLayer.h"
#ifdef HZR_INCLUDE_METAL

#ifdef HZR_PLATFORM_IOS

    #import "Ios/HazardViewController.h"
    #import <Metal/Metal.h>
    #import <QuartzCore/CAMetalLayer.h>
    #import <CoreGraphics/CGColor.h>
    #import <Foundation/Foundation.h>
    #import <UIKit/UIKit.h>

    namespace HazardRenderer::Metal
    {
        MetalWindowLayer::MetalWindowLayer(Window& window, MTL::Device* device)
        {
            id<MTLDevice> dev = (__bridge id<MTLDevice>)device;
            
            CGSize size = [[UIScreen mainScreen] nativeBounds].size;
            
            UIApplication* app = [UIApplication sharedApplication];
            HazardViewController* controller = (HazardViewController*)[[[app delegate] window] rootViewController];
            
            MTKView* view = [controller GetMTKView];
            view.drawableSize = size;
            view.backgroundColor = [UIColor whiteColor];
            view.device = dev;
            
            m_Width = size.width;
            m_Height = size.height;
            
            m_Layer = (__bridge CA::MetalLayer*)view.layer;
        }
        void MetalWindowLayer::Resize(uint32_t width, uint32_t height)
        {
            m_Width = width;
            m_Height = height;
            
            CAMetalLayer* metalLayer = (__bridge CAMetalLayer*)m_Layer;
            
            metalLayer.frame.size = { (CGFloat)m_Width, (CGFloat)m_Height };
            metalLayer.drawableSize = metalLayer.frame.size;
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

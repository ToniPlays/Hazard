//
//  IOSUtils.cpp
//  Hazard-Renderer
//
//  Created by Toni Simoska on 21.11.2024.
//

#include "IOSUtils.h"
#import <UIKit/UIKit.h>

namespace HazardRenderer
{
    Extent IOSUtils::GetWindowSize(uint32_t window)
    {
        auto size = [[UIScreen screens][window] nativeBounds].size;
        return { static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height), 1 };
    }

    SafeArea IOSUtils::GetSafeArea(uint32_t window)
    {
        UIApplication* app = [UIApplication sharedApplication];
        auto i = app.windows[window].safeAreaInsets;
            
        return { static_cast<uint32_t>(i.top), static_cast<uint32_t>(i.right), static_cast<uint32_t>(i.bottom), static_cast<uint32_t>(i.left) };
    }

    Orientation IOSUtils::GetWindowOrientation(uint32_t window)
    {
        auto orientation = [[UIApplication sharedApplication] statusBarOrientation];
        switch(orientation)
        {
            case UIInterfaceOrientationPortrait:
                return Orientation::Portrait;
            case UIInterfaceOrientationPortraitUpsideDown:
                return Orientation::PortraitInverse;
            case UIInterfaceOrientationLandscapeLeft:
                return Orientation::LandscapeLeft;
            case UIInterfaceOrientationLandscapeRight:
                return Orientation::LandscapeRight;
            default: return (Orientation)0;
        }
    }
    bool IOSUtils::IsOrientedAs(uint32_t window, Orientation orientation)
    {
        return (uint8_t)GetWindowOrientation(window) & (uint8_t)orientation;
    }
}

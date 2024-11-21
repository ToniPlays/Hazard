#pragma once

#include "Core/Window.h"
#include "Utility/Callback.h"

namespace HazardRenderer
{
    class IOSUtils
    {
    public:
        static Extent GetWindowSize(uint32_t window);
        static SafeArea GetSafeArea(uint32_t window);
        static Orientation GetWindowOrientation(uint32_t window);
        static bool IsOrientedAs(uint32_t window, Orientation orientation);
        
        static void RegisterRotationChange(const std::function<void(uint8_t)> callback)
        {
            s_OrientationCallbacks.Add(callback);
        }
        
        static void OnDeviceOrientationChanged(Orientation orientation)
        {
            s_OrientationCallbacks.Invoke((uint8_t)orientation);
        }
    private:
        inline static Callback<void(uint8_t)> s_OrientationCallbacks;
    };
}

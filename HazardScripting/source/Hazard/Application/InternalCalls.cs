using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        //Application class
        internal static delegate* unmanaged<void> Application_Quit_Native;
        internal static delegate* unmanaged<float> Display_Width_Native;
        internal static delegate* unmanaged<float> Display_Height_Native;
        internal static delegate* unmanaged<Bool32> Display_IsFullscreen_Native;
        internal static delegate* unmanaged<Bool32, void> Display_SetFullscreen_Native;
        internal static delegate* unmanaged<Bool32> Display_IsVsync_Native;
        internal static delegate* unmanaged<Bool32, void> Display_SetVsync_Native;
        internal static delegate* unmanaged<Resolution> Display_GetResolution_Native;
        internal static delegate* unmanaged<NativeArray<Resolution>> Display_GetResolutions_Native;
        internal static delegate* unmanaged<Resolution, void> Display_SetResolution_Native;
    }
}

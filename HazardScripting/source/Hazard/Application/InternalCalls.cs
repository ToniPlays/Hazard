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
        internal static delegate*<void> Application_Quit_Native;
        internal static delegate*<float> Display_Width_Native;
        internal static delegate*<float> Display_Height_Native;
        internal static delegate*<bool> Display_IsFullscreen_Native;
        internal static delegate*<bool, void> Display_SetFullscreen_Native;
        internal static delegate*<bool> Display_IsVsync_Native;
        internal static delegate*<bool, void> Display_SetVsync_Native;
        internal static delegate*<Resolution> Display_GetResolution_Native;
        internal static delegate*<UnmanagedArray> Display_GetResolutions_Native;
        internal static delegate*<Resolution, void> Display_SetResolution_Native;
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal partial class InternalCalls
    {
        //Application class
        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<void> Application_Quit_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float> Display_Width_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float> Display_Height_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<bool> Display_IsFullscreen_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<bool, void> Display_SetFullscreen_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<bool> Display_IsVsync_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<bool, void> Display_SetVsync_Native;
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal partial class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Log_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Info_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Warn_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Error_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Critical_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Debug_Trace_Native(object message);
    }
}

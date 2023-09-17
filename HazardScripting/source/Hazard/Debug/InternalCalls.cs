using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using Coral.Managed.Interop;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        internal static unsafe delegate*<UnmanagedString, void> Debug_Log_Native;
        internal static unsafe delegate*<UnmanagedString, void> Debug_Info_Native;
        internal static unsafe delegate*<UnmanagedString, void> Debug_Warn_Native;
        internal static unsafe delegate*<UnmanagedString, void> Debug_Error_Native;
        internal static unsafe delegate*<UnmanagedString, void> Debug_Critical_Native;
        internal static unsafe delegate*<UnmanagedString, void> Debug_Trace_Native;
    }
}

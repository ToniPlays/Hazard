using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    internal class InternalCalls
    {
        #region Application
        //Application class
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_Quit_Native();


        #endregion
        #region Console
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Log_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Info_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Warn_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Error_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Critical_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Console_Trace_Native(object message);
        #endregion
    }
}

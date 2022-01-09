using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Console
    {
        public static void Log(object message) { Console_Log_Native(message); }
        public static void Info(object message) { Console_Info_Native(message); }
        public static void Warn(object message) { Console_Warn_Native(message); }
        public static void Error(object message) { Console_Error_Native(message); }
        public static void Critical(object message) { Console_Critical_Native(message); }
        public static void Trace(object message) { Console_Trace_Native(message); }
        //Log message on failure
        public static void Assert(bool success, object message) { if(!success) Console_Trace_Native(message); }

        #region InternalCalls
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

using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Debug
    {
        public static void Log(object message) { Debug_Log_Native(message); }
        public static void Info(object message) { Debug_Info_Native(message); }
        public static void Warn(object message) { Debug_Warn_Native(message); }
        public static void Error(object message) { Debug_Error_Native(message); }
        public static void Critical(object message) { Debug_Critical_Native(message); }
        public static void Trace(object message) { Debug_Trace_Native(message); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Log_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Info_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Warn_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Error_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Critical_Native(object message);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Debug_Trace_Native(object message);
    }
}

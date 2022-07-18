using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Console
    {
        public static void Log(object message) { InternalCalls.Console_Log_Native(message); }
        public static void Info(object message) { InternalCalls.Console_Info_Native(message); }
        public static void Warn(object message) { InternalCalls.Console_Warn_Native(message); }
        public static void Error(object message) { InternalCalls.Console_Error_Native(message); }
        public static void Critical(object message) { InternalCalls.Console_Critical_Native(message); }
        public static void Trace(object message) { InternalCalls.Console_Trace_Native(message); }
        //Log message on failure
        public static void Assert(bool success, object message) { if(!success) InternalCalls.Console_Trace_Native(message); }
    }
}

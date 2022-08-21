using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Debug
    {
        public static void Log(string format, params object[] parameters) { InternalCalls.Debug_Log_Native(Formatter.Format(format, parameters)); }
        public static void Info(string format, params object[] parameters) { InternalCalls.Debug_Info_Native(Formatter.Format(format, parameters)); }
        public static void Warn(string format, params object[] parameters) { InternalCalls.Debug_Warn_Native(Formatter.Format(format, parameters)); }
        public static void Error(string format, params object[] parameters) { InternalCalls.Debug_Error_Native(Formatter.Format(format, parameters)); }
        public static void Critical(string format, params object[] parameters) { InternalCalls.Debug_Critical_Native(Formatter.Format(format, parameters)); }
        public static void Trace(string format, params object[] parameters) { InternalCalls.Debug_Trace_Native(Formatter.Format(format, parameters)); }
        public static void Assert(bool success, string format, params object[] parameters) { if (!success) InternalCalls.Debug_Trace_Native(Formatter.Format(format, parameters)); }

        public static void Log(object message) { InternalCalls.Debug_Log_Native(message); }
        public static void Info(object message) { InternalCalls.Debug_Info_Native(message); }
        public static void Warn(object message) { InternalCalls.Debug_Warn_Native(message); }
        public static void Error(object message) { InternalCalls.Debug_Error_Native(message); }
        public static void Critical(object message) { InternalCalls.Debug_Critical_Native(message); }
        public static void Trace(object message) { InternalCalls.Debug_Trace_Native(message); }
        //Log message on failure
        public static void Assert(bool success, object message) { if(!success) InternalCalls.Debug_Trace_Native(message); }
    }
}

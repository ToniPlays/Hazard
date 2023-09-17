using System.Runtime.CompilerServices;
using Coral.Managed.Interop;

namespace Hazard
{
    public class Debug
    {
        public static void Log(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Log_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Info(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Info_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Warn(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Warn_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Error(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Error_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Critical(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Critical_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Trace(string format, params object[] parameters)
        {
            unsafe { InternalCalls.Debug_Trace_Native(UnmanagedString.FromString(Formatter.Format(format, parameters))); }
        }
        public static void Assert(bool success, string format, params object[] parameters)
        {
            if (success) return;
            unsafe
            {
                InternalCalls.Debug_Trace_Native(UnmanagedString.FromString(Formatter.Format(format, parameters)));
            }
        }

        public static void Log(object message)
        {
            unsafe { InternalCalls.Debug_Log_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Info(object message)
        {
            unsafe { InternalCalls.Debug_Info_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Warn(object message)
        {
            unsafe { InternalCalls.Debug_Warn_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Error(object message)
        {
            unsafe { InternalCalls.Debug_Error_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Critical(object message)
        {
            unsafe { InternalCalls.Debug_Critical_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Trace(object message)
        {
            unsafe { InternalCalls.Debug_Trace_Native(UnmanagedString.FromString(Formatter.Format(message))); }
        }
        public static void Assert(bool success, object message)
        {
            if (success) return;

            unsafe
            {
                InternalCalls.Debug_Trace_Native(UnmanagedString.FromString(Formatter.Format(message)));
            }
        }
    }
}

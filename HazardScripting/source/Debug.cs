using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Debug
    {
        public static void Log(object message) { Log_Native(message); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Log_Native(object message);
    }
}

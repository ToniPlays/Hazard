
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Application
    {
        public static void Quit() { Application_Quit_Native(); }

        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_Quit_Native();
        #endregion
    }
}

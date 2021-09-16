
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Application
    {
        //TODO: LINK
        public static void Quit() { Application_Quit_Native(); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Application_Quit_Native();
    }
}

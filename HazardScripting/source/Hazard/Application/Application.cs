
namespace Hazard
{
    public class Application
    {
        public static void Quit()
        {
            unsafe { InternalCalls.Application_Quit_Native(); }
        }
    }
}

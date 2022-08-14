using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Display
    {
        public static float Width {                                 
            get => InternalCalls.Display_Width_Native();
        }
        public static float Height
        {
            get => InternalCalls.Display_Height_Native();
        }
        public static bool Fullscreen { 
            get => InternalCalls.Display_IsFullscreen_Native();
            private set => InternalCalls.Display_SetFullscreen_Native(value);
        }
        public static bool VSync
        {
            get => InternalCalls.Display_IsVsync_Native();
            set => InternalCalls.Display_SetVsync_Native(value);
        }
    }
}

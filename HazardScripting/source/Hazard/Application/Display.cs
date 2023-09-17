using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Display
    {
        public static float Width
        {
            get
            {
                unsafe
                {
                    return InternalCalls.Display_Width_Native();
                }
            }
        }
        public static float Height
        {
            get
            {
                unsafe
                {
                    return InternalCalls.Display_Height_Native();
                }
            }
        }
        public static bool Fullscreen
        {
            get
            {
                unsafe
                {
                    return InternalCalls.Display_IsFullscreen_Native();
                }
            }
            private set
            {
                unsafe
                {

                    InternalCalls.Display_SetFullscreen_Native(value);
                }
            }
        }
        public static bool VSync
        {
            get
            {
                unsafe
                {
                    return InternalCalls.Display_IsVsync_Native();
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.Display_SetVsync_Native(value);
                }
            }
        }
    }
}

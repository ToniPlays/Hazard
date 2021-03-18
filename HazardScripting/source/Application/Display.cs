using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Display
    {
        public static float Width {
            get => Display_Width_Native();
        }
        public static float Height
        {
            get => Display_Height_Native();
        }
        public static bool Fullscreen { 
            get => Display_IsFullscreen_Native();
            private set => Display_SetFullscreen_Native(value);
        }
        public static bool Vsync
        {
            get => Display_IsVsync_Native();
            set => Display_SetVsync_Native(value);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Display_Width_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Display_Height_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Display_IsFullscreen_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Display_SetFullscreen_Native(bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Display_IsVsync_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Display_SetVsync_Native(bool value);
    }
}

using System.Runtime.CompilerServices;

namespace Hazard
{

    public class Input
    {
        public static bool AnyKey() { return Input_AnyKey_Native(); }
        public static bool KeyDown(Key code) { return Input_KeyDown_Native((int)code); }
        public static bool KeyPressed(Key code) { return Input_KeyPressed_Native((int)code); }
        public static bool KeyReleased(Key code) { return Input_KeyReleased_Native((int)code); }

        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_AnyKey_Native();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_KeyDown_Native(int code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_KeyPressed_Native(int code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_KeyReleased_Native(int code);
        #endregion
    }
}

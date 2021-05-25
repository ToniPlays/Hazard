using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Math
    {
        public static float Random() { return Math_Random_Native(); }
        public static float RandomRange(float min, float max) { return Math_RandomRange_Native(min, max); }
        public static float Pow(float value, float pow) { return Math_Pow_Native(value, pow); }
        public static float Sqrt(float value, float pow) { return Math_Sqrt_Native(value); }
        public static float Min(float first, float second) { return Math_Min_Native(first, second); }
        public static float Max(float first, float second) { return Math_Max_Native(first, second); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_Random_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_RandomRange_Native(float min, float max);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_Pow_Native(float value, float pow);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_Sqrt_Native(float value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_Min_Native(float fist, float second);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Math_Max_Native(float fist, float second);
    }
}

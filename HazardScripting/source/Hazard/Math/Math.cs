using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Math
    {
        public static float Random() { return RandomRange(0.0f, 1.0f); }
        public static float RandomRange(float min, float max) { unsafe { return InternalCalls.Math_RandomRange_Native(min, max); } }
        public static float Pow(float value, float pow) { unsafe { return InternalCalls.Math_Pow_Native(value, pow); } }
        public static float Sqrt(float value) { unsafe { return InternalCalls.Math_Sqrt_Native(value); } }
        public static float Min(float first, float second) { return first > second ? second : first; }
        public static float Max(float first, float second) { return first < second ? second : first; }
        public static float Radians(float degrees) { unsafe { return InternalCalls.Math_Radians_Native(degrees); } }

        public static float Sin(float value) { return (float)System.Math.Sin(value); }
        public static float Cos(float value) { return (float)System.Math.Cos(value); }
        public static float Tan(float value) { return (float)System.Math.Tan(value); }
    }
}

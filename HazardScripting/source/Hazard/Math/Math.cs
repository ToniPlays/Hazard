using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Math
    {
        public static float Random() { return InternalCalls.Math_Random_Native(); }
        public static float RandomRange(float min, float max) { return InternalCalls.Math_RandomRange_Native(min, max); }
        public static float Pow(float value, float pow) { return InternalCalls.Math_Pow_Native(value, pow); }
        public static float Sqrt(float value) { return InternalCalls.Math_Sqrt_Native(value); }
        public static float Min(float first, float second) { return first > second ? second : first; }
        public static float Max(float first, float second) { return first < second ? second : first; }
        public static float Radians(float degrees) { return InternalCalls.Math_Radians_Native(degrees); }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    internal partial class InternalCalls
    {
        #region Math

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Math_RandomRange_Native(float min, float max);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Math_Pow_Native(float value, float pow);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Math_Sqrt_Native(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Math_Radians_Native(float degrees);
        #endregion

        #region Time
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Time_GetDelta_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Time_GetUnscaledDelta_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Time_GetSinceStart_Native();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Time_GetDeltaScale_Native();
        #endregion
    }
}

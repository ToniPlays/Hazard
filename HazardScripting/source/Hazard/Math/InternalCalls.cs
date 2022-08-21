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
    }
}

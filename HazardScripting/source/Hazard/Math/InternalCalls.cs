﻿using System;
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

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float, float, float> Math_RandomRange_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float, float, float> Math_Pow_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float, float> Math_Sqrt_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float, float> Math_Radians_Native;

        #endregion

        #region Time

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float> Time_GetDelta_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float> Time_GetUnscaledDelta_Native;

        internal static unsafe delegate*<float> Time_GetSinceStart_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static unsafe delegate*<float> Time_GetDeltaScale_Native;
        #endregion
    }
}

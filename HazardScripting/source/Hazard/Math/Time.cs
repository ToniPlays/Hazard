using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public class Time
    {
        public static float Delta
        {
            get { unsafe { return InternalCalls.Time_GetDelta_Native(); } }
        }
        public static float UnscaledDelta
        {
            get { unsafe { return InternalCalls.Time_GetUnscaledDelta_Native(); } }
        }
        public static float SinceStart
        {
            get { unsafe { return InternalCalls.Time_GetSinceStart_Native(); } }
        }
        public static float DeltaScale
        {
            get { unsafe { return InternalCalls.Time_GetDeltaScale_Native(); } }
            set { unsafe { InternalCalls.Time_SetDeltaScale_Native(value); } }
        }
    }
}

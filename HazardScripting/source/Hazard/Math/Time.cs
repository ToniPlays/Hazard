using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public class Time
    {
        public static float Delta
        {
            get => InternalCalls.Time_GetDelta_Native();
        }
        public static float UnscaledDelta
        {
            get => InternalCalls.Time_GetUnscaledDelta_Native();
        }
        public static float SinceStart
        {
            get => InternalCalls.Time_GetSinceStart_Native();
        }
        public static float DeltaScale
        {
            get => InternalCalls.Time_GetDeltaScale_Native();
        }
    }
}

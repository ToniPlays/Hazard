using System;
using System.Collections.Generic;
using System.Text;

namespace Hazard
{
    public class Time
    {
        public static double Delta
        {
            get => InternalCalls.Time_GetDelta_Native();
        }
        public static double UnscaledDelta
        {
            get => InternalCalls.Time_GetUnscaledDelta_Native();
        }
        public static double SinceStart
        {
            get => InternalCalls.Time_GetSinceStart_Native();
        }
        public static double DeltaScale
        {
            get => InternalCalls.Time_GetDeltaScale_Native();
        }
    }
}

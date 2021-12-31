using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Noise
    {
        public static float Generate(float x, float y) { return Noise_Generate_Internal(x, y); }
        public static float Generate(Vector2 pos) { return Noise_Generate_Internal(pos.x, pos.y); }

        [MethodImpl(MethodImplOptions.InternalCall)] //TODO: LINK
        private static extern float Noise_Generate_Internal(float x, float y);
    }
}

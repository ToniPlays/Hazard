using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.CompilerServices;

namespace Hazard
{
    public class Noise
    {
        [Todo("Binding", Status.Awaiting)]
        public static float Generate(float x, float y) { return 0.0f; }
        [Todo("Binding", Status.Awaiting)]
        public static float Generate(float x, float y, float z) { return 0.0f; }

        public static float Generate(Vector2 pos) { return Generate(pos.x, pos.y); }
        public static float Generate(Vector3 pos) { return Generate(pos.x, pos.y, pos.z); }
    }
}

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public class Vector2
    {
        public float x, y;
        public Vector2(float x, float y) {
            this.x = x;
            this.y = y;
        }

        public static Vector2 zero = new Vector2(0, 0);
        public static Vector2 one = new Vector2(1, 1);
    }
}

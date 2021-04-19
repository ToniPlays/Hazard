using System.Runtime.InteropServices;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float x, y;
        public Vector2(float x, float y) {
            this.x = x;
            this.y = y;
        }

        public override string ToString() { 
            return "(" + x + ", " +  y + ")";
        }

        public static Vector2 zero = new Vector2(0, 0);
        public static Vector2 one = new Vector2(1, 1);
    }
}

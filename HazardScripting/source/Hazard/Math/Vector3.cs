using System.Runtime.InteropServices;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x, y, z;

        public Vector2 xy { 
            get => new Vector2(x, y);
            set { x = value.x; y = value.y; }
        }
        public Vector2 xz
        {
            get => new Vector2(x, z);
            set { x = value.x; z = value.y; }
        }
        public Vector2 yz
        {
            get => new Vector2(y, z);
            set { y = value.x; z = value.y; }
        }

        public Vector3(float scalar) {
            x = scalar;
            y = scalar;
            z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public Vector3(Vector2 value, float z = 0.0f)
        {
            this.x = value.x;
            this.y = value.y;
            this.z = z;
        }

        public override string ToString()
        {
            return "(" + x + ", " + y + ", " + z + ")";
        }

        public static Vector3 zero = new Vector3(0, 0, 0);
        public static Vector3 one = new Vector3(1, 1, 1);
    }
}

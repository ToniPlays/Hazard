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

        public Vector3(float x, float y, float z = 0.0f)
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

        #region Static methods
        static float Distance(Vector3 first, Vector3 second) 
        {
            float dx = first.x - second.x;
            float dy = first.y - second.y;
            float dz = first.z - second.z;

            return Math.Sqrt(dx * dx + dy * dy + dz * dz);
        }

        #endregion
        #region Arithmetic operators
        public static Vector3 operator +(Vector3 left, Vector3 right) {
            return new Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
        }
        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
        }
        public static Vector3 operator *(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
        }
        public static Vector3 operator *(Vector3 left, float right)
        {
            return new Vector3(left.x * right, left.y * right, left.z * right);
        }
        #endregion
        #region Logical operators
        public static bool operator ==(Vector3 left, Vector3 right)
        {
            return left.x == right.x && left.y == right.y && left.z == right.z;
        }
        public static bool operator != (Vector3 left, Vector3 right) {
            return !(left == right);
        }
        #endregion
        public override string ToString()
        {
            return "[" + x + ", " + y + ", " + z + "]";
        }

        public static Vector3 Zero = new Vector3(0, 0, 0);
        public static Vector3 One = new Vector3(1, 1, 1);
        public static Vector3 Up = new Vector3(0, 1, 0);
        public static Vector3 Down = new Vector3(0, -1, 0);
        public static Vector3 Left = new Vector3(-1, 0, 0);
        public static Vector3 Right = new Vector3(1, 0, 0);
        public static Vector3 Front = new Vector3(0, 0, 1);
        public static Vector3 Back = new Vector3(0, 0, -1);
    }
}

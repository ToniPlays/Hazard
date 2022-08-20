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
        #region Static methods
        static float Distance(Vector2 first, Vector2 second)
        {
            float dx = first.x - second.x;
            float dy = first.y - second.y;

            return Math.Sqrt(dx * dx + dy * dy);
        }

        #endregion
        #region Arithmetic operators
        public static Vector2 operator +(Vector2 left, Vector3 right)
        {
            return new Vector2(left.x + right.x, left.y + right.y);
        }
        public static Vector2 operator -(Vector2 left, Vector3 right)
        {
            return new Vector2(left.x - right.x, left.y - right.y);
        }
        public static Vector2 operator *(Vector2 left, Vector3 right)
        {
            return new Vector2(left.x * right.x, left.y * right.y);
        }
        public static Vector2 operator *(Vector2 left, float right)
        {
            return new Vector2(left.x * right, left.y * right);
        }
        #endregion
        #region Logical operators
        public static bool operator ==(Vector2 left, Vector2 right)
        {
            return left.x == right.x && left.y == right.y;
        }
        public static bool operator !=(Vector2 left, Vector2 right)
        {
            return !(left == right);
        }
        #endregion
        public override string ToString()
        {
            return "[" + x + ", " + y + "]";
        }

        public static Vector2 zero = new Vector2(0, 0);
        public static Vector2 one = new Vector2(1, 1);
    }
}

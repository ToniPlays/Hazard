using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public class Vector3
    {
        public float x, y, z;
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 zero = new Vector3(0, 0, 0);
    }
}

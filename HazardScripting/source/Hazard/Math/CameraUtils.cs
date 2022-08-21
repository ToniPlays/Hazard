using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public enum Projection { Perspective = 0, Orthographic = 1 }

    public struct ClippingPlane
    {
        public float ZNear;
        public float ZFar;

        public ClippingPlane(float zNear, float zFar)
        {
            ZNear = zNear;
            ZFar = zFar;
        }
        public override string ToString()
        {
            return String.Format("[ZNear: {0}, ZFar: {1}]", ZNear, ZFar);
        }
    }
}

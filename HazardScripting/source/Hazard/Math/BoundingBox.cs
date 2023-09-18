using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public struct BoundingBox
    {
        float MinX;
        float MinY;
        float MinZ;
        float MaxX;
        float MaxY;
        float MaxZ;

        public bool Collides(Vector3 point)
        {
            return 
                MinX < point.x && point.x < MaxX &&
                MinY < point.y && point.y < MaxY &&
                MinZ < point.z && point.z < MaxZ;
        }
    }
}

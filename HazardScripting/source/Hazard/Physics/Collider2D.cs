using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Hazard
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Collider2D
    {
        private ulong entityID;

        public Entity parent {
            get
            {
                return new Entity(entityID);
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class MeshComponent : Component
    {
        

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void MeshComponent_SetMesh_Native(ulong id);
    }
}

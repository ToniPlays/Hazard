using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Hazard.Rendering;

namespace Hazard
{
    public class MeshComponent : Component
    {
        public void SetMesh(Mesh mesh)
        {
            MeshComponent_SetMesh_Native(parent.ID, mesh.GetID());
        }

        #region InternalCalls
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshComponent_SetMesh_Native(ulong id, ulong meshID);
        #endregion
    }
}

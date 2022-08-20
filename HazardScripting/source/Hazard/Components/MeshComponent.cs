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
        internal MeshComponent(ulong ID) : base(ID) { }
        public void SetMesh(Mesh mesh)
        {
            InternalCalls.MeshComponent_SetMesh_Native(parentEntity.ID, mesh.GetID());
        }
        public bool IsActive() { return InternalCalls.Component_IsActive_Native(parentEntity.ID, typeof(MeshComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(parentEntity.ID, active, typeof(MeshComponent)); }
    }
}

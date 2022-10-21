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
        public MeshComponent() : base(0) { }
        internal MeshComponent(ulong ID) : base(ID) { }
        public void SetMesh(Mesh mesh)
        {
            InternalCalls.MeshComponent_SetMesh_Native(ParentEntity.ID, mesh.ID);
        }
        public bool IsActive() { return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(MeshComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(MeshComponent)); }
    }
}

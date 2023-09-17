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
        public Mesh Mesh
        {
            get
            {
                unsafe
                {
                    ulong id = InternalCalls.MeshComponent_GetMesh_Native(ParentEntity.ID);
                    return new Mesh(id);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.MeshComponent_SetMesh_Native(ParentEntity.ID, value.ID);
                }
            }
        }
        public bool IsActive()
        {
            unsafe
            {
                return InternalCalls.Component_IsActive_Native(ParentEntity.ID, typeof(MeshComponent));
            }
        }
        public void SetActive(bool active)
        {
            unsafe
            {
                InternalCalls.Component_SetActive_Native(ParentEntity.ID, active, typeof(MeshComponent));
            }
        }
    }
}

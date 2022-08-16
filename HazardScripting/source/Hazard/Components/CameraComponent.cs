using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hazard
{
    public class CameraComponent : Component
    {
        public CameraComponent() : base(0) { }
        internal CameraComponent(ulong ID) : base(ID) { }

        public float FOV 
        {
            get => InternalCalls.CameraComponent_GetFOV_Native(parentEntity.ID);
            set => InternalCalls.CameraComponent_SetFOV_Native(parentEntity.ID, value);
        }

        public bool IsActive() { return InternalCalls.Component_IsActive_Native(parentEntity.ID, typeof(CameraComponent)); }
        public void SetActive(bool active) { InternalCalls.Component_SetActive_Native(parentEntity.ID, active, typeof(CameraComponent)); }
    }
}

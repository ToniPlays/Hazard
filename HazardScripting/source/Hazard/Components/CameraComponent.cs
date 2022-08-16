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
    }
}

using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TransformComponent : Component
    {
        public Vector3 position
        {
            get
            {
                InternalCalls.Transform_GetPosition_Native(parent.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetPosition_Native(parent.ID, ref value);
        }
        public Vector3 rotation
        {
            get
            {
                InternalCalls.Transform_GetRotation_Native(parent.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetRotation_Native(parent.ID, ref value);
        }
        public Vector3 scale
        {
            get
            {
                InternalCalls.Transform_GetScale_Native(parent.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetScale_Native(parent.ID, ref value);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TransformComponent : Component
    {
        public Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPosition_Native(parentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetPosition_Native(parentEntity.ID, ref value);
        }
        public Vector3 Rotation
        {
            get
            {
                InternalCalls.Transform_GetRotation_Native(parentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetRotation_Native(parentEntity.ID, ref value);
        }
        public Vector3 Scale
        {
            get
            {
                InternalCalls.Transform_GetScale_Native(parentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.Transform_SetScale_Native(parentEntity.ID, ref value);
        }
    }
}

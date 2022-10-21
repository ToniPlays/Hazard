using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TransformComponent : Component
    {
        public TransformComponent() : base(0) { }
        internal TransformComponent(ulong ID) : base(ID) { }
        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition_Native(ParentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.TransformComponent_SetPosition_Native(ParentEntity.ID, ref value);
        }
        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation_Native(ParentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.TransformComponent_SetRotation_Native(ParentEntity.ID, ref value);
        }
        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale_Native(ParentEntity.ID, out Vector3 result);
                return result;
            }
            set => InternalCalls.TransformComponent_SetScale_Native(ParentEntity.ID, ref value);
        }
    }
}

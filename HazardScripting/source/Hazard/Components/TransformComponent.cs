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
                unsafe
                {
                    return InternalCalls.TransformComponent_GetPosition_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {

                    InternalCalls.TransformComponent_SetPosition_Native(ParentEntity.ID, value);
                }
            }
        }
        public Vector3 Rotation
        {
            get
            {
                unsafe
                {
                    return InternalCalls.TransformComponent_GetRotation_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.TransformComponent_SetRotation_Native(ParentEntity.ID, value);
                }
            }
        }
        public Vector3 Scale
        {
            get
            {
                unsafe
                {
                    return InternalCalls.TransformComponent_GetScale_Native(ParentEntity.ID);
                }
            }
            set
            {
                unsafe
                {
                    InternalCalls.TransformComponent_SetScale_Native(ParentEntity.ID, value);
                }
            }
        }
    }
}

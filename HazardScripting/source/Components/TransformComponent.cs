using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;

namespace Hazard
{
    public class TransformComponent : Component
    {
        //TODO: LINK
        public Vector3 position {
            get {
                Transform_GetPosition_Native(parent.ID, out Vector3 result);
                return result;
            }
            set
            {
                Transform_SetPosition_Native(parent.ID, ref value);
            }
        }
        //TODO: LINK
        public Vector3 rotation {
            get
            {
                Transform_GetRotation_Native(parent.ID, out Vector3 result);
                return result;
            }
            set
            {
                Transform_SetRotation_Native(parent.ID, ref value);
            }
        }
        //TODO: LINK
        public Vector3 scale {
            get
            {
                Transform_GetScale_Native(parent.ID, out Vector3 result);
                return result;
            }
            set
            {
                Transform_SetScale_Native(parent.ID, ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_GetPosition_Native(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_SetPosition_Native(ulong id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_GetRotation_Native(ulong id, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_SetRotation_Native(ulong id, ref Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_GetScale_Native(ulong id, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Transform_SetScale_Native(ulong id, ref Vector3 scale);
    }
}

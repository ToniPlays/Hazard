using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal partial class InternalCalls
    {
        #region TagComponent
        //Tag component
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Tag_GetName_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Tag_SetName_Native(ulong id, string name);
        #endregion
        #region TransformComponent
        //Transform component
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetPosition_Native(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetPosition_Native(ulong id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetRotation_Native(ulong id, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetRotation_Native(ulong id, ref Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_GetScale_Native(ulong id, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Transform_SetScale_Native(ulong id, ref Vector3 scale);

        #endregion
        #region MeshComponent

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshComponent_SetMesh_Native(ulong id, ulong meshID);

        #endregion
    }
}

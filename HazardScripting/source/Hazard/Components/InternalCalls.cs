using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal partial class InternalCalls
    {
        #region CameraComponent
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float CameraComponent_GetFOV_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float CameraComponent_SetFOV_Native(ulong id, float fov);
        #endregion
        #region MeshComponent

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshComponent_SetMesh_Native(ulong id, ulong meshID);

        #endregion
        #region SpriteRenderer

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SpriteRendererComponent_GetColor_Native(ulong id, out Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SpriteRendererComponent_SetColor_Native(ulong id, ref Color color);

        #endregion
        #region TagComponent
        //Tag component
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string TagComponent_GetName_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TagComponent_SetName_Native(ulong id, string name);
        #endregion
        #region TransformComponent
        //Transform component
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetPosition_Native(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetPosition_Native(ulong id, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetRotation_Native(ulong id, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetRotation_Native(ulong id, ref Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_GetScale_Native(ulong id, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void TransformComponent_SetScale_Native(ulong id, ref Vector3 scale);

        #endregion
    }
}

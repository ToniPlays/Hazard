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
        internal static extern int CameraComponent_GetProjection_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CameraComponent_SetProjection_Native(ulong id, int projection);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CameraComponent_GetClipping_Native(ulong id, out Vector2 clipping);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CameraComponent_SetClipping_Native(ulong id, float near, float far);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float CameraComponent_GetFOV_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CameraComponent_SetFOV_Native(ulong id, float fov);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float CameraComponent_GetOrthoSize_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CameraComponent_SetOrthoSize_Native(ulong id, float size);

        #endregion
        #region MeshComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong MeshComponent_GetMesh_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshComponent_SetMesh_Native(ulong id, ulong meshID);

        #endregion
        #region SpriteRenderer

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SpriteRendererComponent_GetColor_Native(ulong id, out Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SpriteRendererComponent_SetColor_Native(ulong id, ref Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong SpriteRendererComponent_GetSprite_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SpriteRendererComponent_SetSprite_Native(ulong id, ulong handle);

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

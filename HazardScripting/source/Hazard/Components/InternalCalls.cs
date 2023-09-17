using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal static unsafe partial class InternalCalls
    {
        #region CameraComponent

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, int> CameraComponent_GetProjection_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, int, void> CameraComponent_SetProjection_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Vector2> CameraComponent_GetClipping_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float, float, void> CameraComponent_SetClipping_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float> CameraComponent_GetFOV_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float, void> CameraComponent_SetFOV_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float> CameraComponent_GetOrthoSize_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float, void> CameraComponent_SetOrthoSize_Native;

        #endregion
        #region MeshComponent

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong> MeshComponent_GetMesh_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong, void> MeshComponent_SetMesh_Native;

        #endregion
        #region SpriteRenderer

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Color> SpriteRendererComponent_GetColor_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, Color, void> SpriteRendererComponent_SetColor_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong> SpriteRendererComponent_GetSprite_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong, void> SpriteRendererComponent_SetSprite_Native;

        #endregion
        #region TagComponent
        //Tag component
        internal static delegate*<ulong, UnmanagedString> TagComponent_GetName_Native;

        internal static delegate*<ulong, UnmanagedString, void> TagComponent_SetName_Native;
        #endregion
        #region TransformComponent
        //Transform component
        internal static delegate*<ulong, Vector3> TransformComponent_GetPosition_Native;
        internal static delegate*<ulong, Vector3, void> TransformComponent_SetPosition_Native;

        internal static delegate*<ulong, Vector3> TransformComponent_GetRotation_Native;
        internal static delegate*<ulong, Vector3, void> TransformComponent_SetRotation_Native;

        internal static delegate*<ulong, Vector3> TransformComponent_GetScale_Native;
        internal static delegate*<ulong, Vector3, void> TransformComponent_SetScale_Native;

        #endregion
        #region SkyLightComponent

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float> SkyLightComponent_GetIntensity_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, float, void> SkyLightComponent_SetIntensity_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong> SkyLightComponent_GetEnvironmentMap_Native;

        [Todo("Binding", Status.Awaiting)]
        internal static delegate*<ulong, ulong, void> SkyLightComponent_SetEnvironmentMap_Native;
        #endregion
    }
}

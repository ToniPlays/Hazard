using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Hazard
{
    internal partial class InternalCalls
    {
        //Tag component
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Tag_GetName_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Tag_SetName_Native(ulong id, string name);

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

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void MeshComponent_SetMesh_Native(ulong id, ulong meshID);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AudioSource_Play_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AudioSource_Stop_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AudioSource_Pause_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float AudioSource_GetGain_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AudioSource_SetGain_Native(ulong id, float value);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float AudioSource_GetPitch_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void AudioSource_SetPitch_Native(ulong id, float value);


        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AudioSource_IsLoaded_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AudioSource_IsLooping_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AudioSource_SetLooping_Native(ulong id, bool value);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AudioSource_IsSpatial_Native(ulong id);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AudioSource_SetSpatial_Native(ulong id, bool value);

        [Todo("Binding", Status.Awaiting)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float AudioSource_GetDuration_Native(ulong id);

    }
}

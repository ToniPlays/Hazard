using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class AudioSourceComponent : Component
    {
        public float gain 
        {
            get => AudioSource_GetGain_Native(parent.ID);
            set => AudioSource_SetGain_Native(parent.ID, value);
        }

        public float pitch 
        {
            get => AudioSource_GetPitch_Native(parent.ID);
            set => AudioSource_SetPitch_Native(parent.ID, value);
        }

        public void Play() { AudioSource_Play_Native(parent.ID); }
        public void Stop() { AudioSource_Stop_Native(parent.ID); }
        public void Pause() { AudioSource_Pause_Native(parent.ID); }

        public bool IsLoaded() { return AudioSource_IsLoaded_Native(parent.ID); }
        public bool IsLooping() { return AudioSource_IsLooping_Native(parent.ID); }
        public void SetLooping(bool looping) { AudioSource_SetLooping_Native(parent.ID, looping); }
        public bool IsSpatial() { return AudioSource_IsSpatial_Native(parent.ID); }
        public void SetSpatial(bool spatial) { AudioSource_SetSpatial_Native(parent.ID, spatial); }
        public float GetDuration() { return AudioSource_GetDuration_Native(parent.ID); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Play_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Stop_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Pause_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetGain_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetGain_Native(ulong id, float value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetPitch_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetPitch_Native(ulong id, float value);


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AudioSource_IsLoaded_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AudioSource_IsLooping_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AudioSource_SetLooping_Native(ulong id, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AudioSource_IsSpatial_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AudioSource_SetSpatial_Native(ulong id, bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetDuration_Native(ulong id);
    }
}

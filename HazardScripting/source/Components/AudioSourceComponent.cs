using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class AudioSourceComponent : Component
    {

        public float gain {
            get => AudioSource_GetGain_Native(parent.ID);
            set => AudioSource_SetGain_Native(parent.ID, value);
        }
        public float pitch {
            get => AudioSource_GetPitch_Native(parent.ID);
            set => AudioSource_SetPitch_Native(parent.ID, value);
        }

        public void Play() { AudioSource_Play_Native(parent.ID); }
        public void Stop() { AudioSource_Stop_Native(parent.ID); }

        public void Pause() { AudioSource_Pause_Native(parent.ID); }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetGain_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetGain_Native(ulong id, float value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetPitch_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetPitch_Native(ulong id, float value);


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Play_Native(ulong id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Stop_Native(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Pause_Native(ulong id);
    }
}

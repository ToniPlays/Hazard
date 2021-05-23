using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    class AudioSource : Component
    {

        public float gain {
            get => AudioSource_GetGain_Internal();
            set => AudioSource_SetGain_Internal(value);
        }
        public float pich
        {
            get => AudioSource_GetPitch_Internal();
            set => AudioSource_SetPitch_Internal(value);
        }


        public void Play() { AudioSource_Play_Internal(); }
        public void Stop() { AudioSource_Stop_Internal(); }

        public void Pause() { AudioSource_Pause_Internal(); }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetGain_Internal();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetGain_Internal(float value);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float AudioSource_GetPitch_Internal();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_SetPitch_Internal(float value);


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Play_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Stop_Internal();
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Pause_Internal();
    }
}

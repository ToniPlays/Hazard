using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    class AudioSource : Component
    {
        public void Play() { AudioSource_Play_Internal(); }
        public void Stop() { AudioSource_Stop_Internal(); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Play_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AudioSource_Stop_Internal();
    }
}

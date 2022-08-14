using System;
using System.Runtime.CompilerServices;

namespace Hazard
{
    public class AudioSourceComponent : Component
    {
        public float Gain
        {
            get => InternalCalls.AudioSource_GetGain_Native(parentEntity.ID);
            set => InternalCalls.AudioSource_SetGain_Native(parentEntity.ID, value);
        }

        public float Pitch
        {
            get => InternalCalls.AudioSource_GetPitch_Native(parentEntity.ID);
            set => InternalCalls.AudioSource_SetPitch_Native(parentEntity.ID, value);
        }

        public void Play() { InternalCalls.AudioSource_Play_Native(parentEntity.ID); }
        public void Stop() { InternalCalls.AudioSource_Stop_Native(parentEntity.ID); }
        public void Pause() { InternalCalls.AudioSource_Pause_Native(parentEntity.ID); }

        public bool IsLoaded() { return InternalCalls.AudioSource_IsLoaded_Native(parentEntity.ID); }
        public bool IsLooping() { return InternalCalls.AudioSource_IsLooping_Native(parentEntity.ID); }
        public void SetLooping(bool looping) { InternalCalls.AudioSource_SetLooping_Native(parentEntity.ID, looping); }
        public bool IsSpatial() { return InternalCalls.AudioSource_IsSpatial_Native(parentEntity.ID); }
        public void SetSpatial(bool spatial) { InternalCalls.AudioSource_SetSpatial_Native(parentEntity.ID, spatial); }
        public float GetDuration() { return InternalCalls.AudioSource_GetDuration_Native(parentEntity.ID); }
    }
}

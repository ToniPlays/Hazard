#pragma once

#include <hzrpch.h>
#include "AudioSource.h"
#include "Hazard/Assets/AssetManager.h"

// #include <AL/al.h>
// #include <AL/alext.h>

namespace Hazard::Audio 
{
    AudioSource::AudioSource() 
    {
        // alGenSources(1, &m_Source);
    }
    AudioSource::AudioSource(Ref<AudioBufferData> buffer)
    {
        // alGenSources(1, &m_Source);
        // SetSourceBuffer(buffer);
    }
    AudioSource::~AudioSource()
    {
        // Stop();
    }
    void AudioSource::Play()
    {
        // alSourcePlay(m_Source);
    }
    void AudioSource::Stop()
    {
        // alSourceStop(m_Source);
    }
    void AudioSource::Pause()
    {
        // alSourcePause(m_Source);
    }
    void AudioSource::SetPosition(glm::vec3 pos)
    {
        // m_Pos = pos;
        // alSource3f(m_Source, AL_POSITION, pos.x, pos.y, pos.z);
    }
    void AudioSource::SetGain(float gain)
    {
        // m_Gain = gain;
        // alSourcef(m_Source, AL_GAIN, gain);
    }
    void AudioSource::SetPitch(float pitch)
    {
        //  m_Pitch = pitch;
        // alSourcef(m_Source, AL_PITCH, pitch);
    }
    void AudioSource::SetSpatial(bool spatial)
    {
        // m_Spatial = spatial;
        // alSourcei(m_Source, AL_SOURCE_SPATIALIZE_SOFT, spatial);
        // alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    }
    void AudioSource::SetLoop(bool loop)
    {
        // m_Looping = loop;
        // alSourcei(m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }
    void AudioSource::SetSourceBuffer(Ref<AudioBufferData> buffer)
    {
        // m_AudioSourceBuffer = buffer;
        // alSourcei(m_Source, AL_BUFFER, m_AudioSourceBuffer->bufferID);
    }
    std::pair<uint32_t, uint32_t> AudioSource::GetLength() const
    {
        // //Return audio duration in minutes and seconds
        // return { (uint32_t)(m_AudioSourceBuffer->LenSec / 60.0f), (uint32_t)m_AudioSourceBuffer->LenSec % 60 };
    }
}
#pragma once

#include <hzrpch.h>
#include "AudioClip.h"

#include <AL/al.h>
#include <AL/alext.h>

namespace Hazard::Audio 
{
    AudioClip::AudioClip(uint32_t buffer, bool loaded, float length) 
    {
        m_Buffer = buffer;
        m_Loaded = loaded;
        m_TotalDuration = length;
    }

    AudioClip::~AudioClip()
    {

    }
    void AudioClip::Play()
    {
        alSourcePlay(m_Source);
    }
    void AudioClip::Stop()
    {
        alSourceStop(m_Source);
    }
    void AudioClip::Pause()
    {
        alSourcePause(m_Source);
    }
    void AudioClip::SetPosition(glm::vec3 pos)
    {
        m_Pos = pos;
        alSource3f(m_Source, AL_POSITION, pos.x, pos.y, pos.z);
    }
    void AudioClip::SetGain(float gain)
    {
        m_Gain = gain;
        alSourcef(m_Source, AL_GAIN, gain);
    }
    void AudioClip::SetPitch(float pitch)
    {
         m_Pitch = pitch;
        alSourcef(m_Source, AL_PITCH, pitch);
    }
    void AudioClip::SetSpatial(bool spatial)
    {
        m_Spatial = spatial;
        alSourcei(m_Source, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    }
    void AudioClip::SetLoop(bool loop)
    {
        m_Looping = loop;
        alSourcei(m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }
    std::pair<uint32_t, uint32_t> AudioClip::GetLength() const
    {
        return { (uint32_t)(m_TotalDuration / 60.0f), (uint32_t)m_TotalDuration % 60 };
    }
    AudioClip AudioClip::LoadFromFile(const std::string& file, bool spatial)
    {
        AudioClip result;
        result.SetSpatial(spatial);
        return result;
    }
}

#include <hzrpch.h>
#include "AudioSource.h"
#include "Hazard/Assets/AssetManager.h"

#include <AL/al.h>
#include <AL/alext.h>

namespace Hazard::Audio 
{
    AudioSource::AudioSource() 
    {
#ifdef HZR_INCLUDE_OPENAL
        alGenSources(1, &m_Source);
#endif
    }
    AudioSource::AudioSource(Ref<AudioBufferData> buffer)
    {
#ifdef HZR_INCLUDE_OPENAL
        alGenSources(1, &m_Source);
#endif
        SetSourceBuffer(buffer);
    }
    AudioSource::~AudioSource()
    {
        Stop();
    }
    void AudioSource::Play()
    {
#ifdef HZR_INCLUDE_OPENAL
        alSourcePlay(m_Source);
#endif
    }
    void AudioSource::Stop()
    {
#ifdef HZR_INCLUDE_OPENAL
        alSourceStop(m_Source);
#endif
    }
    void AudioSource::Pause()
    {
#ifdef HZR_INCLUDE_OPENAL
        alSourcePause(m_Source);
#endif
    }
    void AudioSource::SetPosition(glm::vec3 pos)
    {
        m_Pos = pos;
#ifdef HZR_INCLUDE_OPENAL
        alSource3f(m_Source, AL_POSITION, pos.x, pos.y, pos.z);
#endif
    }
    void AudioSource::SetGain(float gain)
    {
        m_Gain = gain;
#ifdef HZR_INCLUDE_OPENAL
        alSourcef(m_Source, AL_GAIN, gain);
#endif
    }
    void AudioSource::SetPitch(float pitch)
    {
        m_Pitch = pitch;
#ifdef HZR_INCLUDE_OPENAL
        alSourcef(m_Source, AL_PITCH, pitch);
#endif
    }
    void AudioSource::SetSpatial(bool spatial)
    {
        m_Spatial = spatial;
#ifdef HZR_INCLUDE_OPENAL
        alSourcei(m_Source, AL_SOURCE_SPATIALIZE_SOFT, spatial);
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
#endif
    }
    void AudioSource::SetLoop(bool loop)
    {
        m_Looping = loop;
#ifdef HZR_INCLUDE_OPENAL
        alSourcei(m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
#endif
    }
    void AudioSource::SetSourceBuffer(Ref<AudioBufferData> buffer)
    {
        m_AudioSourceBuffer = buffer;
#ifdef HZR_INCLUDE_OPENAL
        alSourcei(m_Source, AL_BUFFER, m_AudioSourceBuffer->bufferID);
#endif
    }
    std::pair<uint32_t, uint32_t> AudioSource::GetLength() const
    {
        //Return audio duration in minutes and seconds
        return { (uint32_t)(m_AudioSourceBuffer->LenSec / 60.0f), (uint32_t)m_AudioSourceBuffer->LenSec % 60 };
    }
}

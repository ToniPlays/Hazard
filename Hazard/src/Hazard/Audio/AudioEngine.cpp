#pragma once

#include <hzrpch.h>
#include "AudioEngine.h"
#include "AudioClip.h"
#include "alhelpers.h"

#include "AL/al.h"
#include <AL/alc.h>
#include "AL/alext.h"
#include "alhelpers.h"

#include "AudioLoader.h"

namespace Hazard::Audio 
{
    AudioEngine::AudioEngine() : Module::Module("AudioEngine")
    {

    }
    void AudioEngine::InitAudio(AudioEngineCreateInfo* info)
    {
        HZR_PROFILE_FUNCTION();

        if (InitAL(m_AudioDevice, nullptr, 0) != 0) {
            HZR_THROW("[Hazard Audio]: Failed to init");
            return;
        }
        AudioLoader::Init();
    }
    void AudioEngine::Close()
    {

    }
}
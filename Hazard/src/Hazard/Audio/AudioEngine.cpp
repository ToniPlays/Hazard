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
    AudioEngine::AudioEngine(AudioEngineCreateInfo* info) : Module::Module("AudioEngine")
    {
        HZR_PROFILE_FUNCTION();
        int result = InitAL(m_AudioDevice, nullptr, 0);
        HZR_CORE_ASSERT(result == 0, "[Hazard Audio]: Failed to init");

        AudioLoader::Init();
    }
    void AudioEngine::Close()
    {
        HZR_PROFILE_FUNCTION();
    }
}
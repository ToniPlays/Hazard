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

    ALCdevice* m_AudioDevice = nullptr;

    AudioEngine::AudioEngine() : Module::Module("AudioEngine")
    {

    }
    AudioEngine::~AudioEngine()
    {

    }
    void AudioEngine::InitAudio(AudioEngineCreateInfo* info)
    {
        HZR_PROFILE_FUNCTION();
        HZR_CORE_INFO("Audio engine init");

        if (InitAL(m_AudioDevice, nullptr, 0) != 0) {
            HZR_THROW("[Hazard Audio]: Failed to init");
            return;
        }
        AudioLoader::Init();
    }
    void AudioEngine::Update()
    {

    }
    void AudioEngine::Close()
    {

    }
}
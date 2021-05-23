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
            HZR_THROW("Failed to init audio engine");
            return;
        }
        AudioLoader::Init();

        AudioClip clip = AudioLoader::LoadFile("res/audio/Axel Wernberg Infinity.mp3");
        clip.SetGain(0.5);
        clip.Play();
    }
    void AudioEngine::Update()
    {

    }
    void AudioEngine::Close()
    {

    }
}
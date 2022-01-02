
#include <hzrpch.h>
#include "AudioEngine.h"
#include "alhelpers.h"

 #include "AL/al.h"
 #include <AL/alc.h>
 #include "AL/alext.h"
#include "alhelpers.h"

#include "AudioLoader.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard::Audio
{
	AudioEngine::AudioEngine(AudioEngineCreateInfo* info) : Module::Module("AudioEngine")
	{
		HZR_PROFILE_FUNCTION();
#ifdef HZR_INCLUDE_OPENAL
		 int result = InitAL(m_AudioDevice, nullptr, 0);

		 // Init default listener
		 ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
		 ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
		 ALfloat listenerOri[] = { 0.0, 0.0,-1.0, 0.0, 1.0, 0.0 };
		 alListenerfv(AL_POSITION, listenerPos);
		 alListenerfv(AL_VELOCITY, listenerVel);
		 alListenerfv(AL_ORIENTATION, listenerOri);

		 HZR_CORE_ASSERT(result == 0, "[Hazard Audio]: Failed to init");

		 AssetManager::RegisterLoader<AudioLoader>(AssetType::AudioClip);
#endif
	}
	void AudioEngine::Close()
	{
		HZR_PROFILE_FUNCTION();
	}
}

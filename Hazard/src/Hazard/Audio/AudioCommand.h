#pragma once

#include "Hazard/Entity/Component.h"
#include "Hazard/Audio/AudioLoader.h"

namespace Hazard::Audio 
{
	class AudioCommand 
	{
	public:
		static Ref<AudioClip> Create(AudioClipCreateInfo* createInfo);
		static void StopAll();

		static std::vector<Ref<AudioClip>> GetAudioClips() { return AudioLoader::GetAllClips(); }

	private:
	};
}
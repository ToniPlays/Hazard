#pragma once

#include "Hazard/Entity/Component.h"
#include "Hazard/Audio/AudioLoader.h"

namespace Hazard::Audio 
{
	class AudioCommand 
	{
	public:
		static void Create(const std::string& fileName);
		static void StopAll();

		static std::vector<Ref<AudioClip>> GetAudioClips() { return AudioLoader::GetAllClips(); }

	private:
	};
}
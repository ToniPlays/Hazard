#pragma once

#include "Hazard/Module.h"
#include "AL/alc.h"

namespace Hazard::Audio {
	class AudioEngine : public Module::Module
	{
	public:
		AudioEngine();
		~AudioEngine();

		void InitAudio(AudioEngineCreateInfo* info);

		void Update() override;
		void Close() override;
		
	};
}
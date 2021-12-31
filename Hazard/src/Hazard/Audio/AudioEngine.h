#pragma once

#include "Hazard/Module.h"
// #include "AL/alc.h"

namespace Hazard::Audio 
{
	class AudioEngine : public Module::Module
	{
	public:
		AudioEngine(AudioEngineCreateInfo* info);
		~AudioEngine() = default;

		void Close() override;
	private:
		// ALCdevice* m_AudioDevice = nullptr;
	};
}
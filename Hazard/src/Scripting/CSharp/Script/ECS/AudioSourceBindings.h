#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind debug calls internal
	class AudioSourceBindings {
	public:
		static BindMap GetBindings();

		static void AudioSource_Play_Native(uint32_t id);
		static void AudioSource_Stop_Native(uint32_t id);
		static void AudioSource_Pause_Native(uint32_t id);

		static float AudioSource_GetGain_Native(uint32_t id);
		static void AudioSource_SetGain_Native(uint32_t id, float value);

		static float AudioSource_GetPitch_Native(uint32_t id);
		static void AudioSource_SetPitch_Native(uint32_t id, float value);
	};
}
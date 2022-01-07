#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind Audio source calls internal
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

		static bool AudioSource_IsLoaded_Native(uint32_t id);
		static bool AudioSource_IsLooping_Native(uint32_t id);
		static void AudioSource_SetLooping_Native(uint32_t id, bool value);

		static bool AudioSource_IsSpatial_Native(uint32_t id);
		static void AudioSource_SetSpatial_Native(uint32_t id, bool value);

		static float AudioSource_GetDuration_Native(uint32_t id);
	};
}
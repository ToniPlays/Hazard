#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind debug calls internal
	class AudioSourceBindings {
	public:
		static BindMap GetBindings();

		static void AudioSource_Play_Native();
		static void AudioSource_Stop_Native();
	};
}
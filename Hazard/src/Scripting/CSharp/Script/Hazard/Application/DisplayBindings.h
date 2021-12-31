#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind aplication functions
	class DisplayBindings {
	public:
		static BindMap GetBindings();

	private:
		static float GetDisplayWidth();
		static float GetDisplayHeight();
		static bool GetDisplayFullscreen();
		static bool GetDisplayVSync();

		static void SetDisplayFullscreen(MonoObject* obj);
		static void SetDisplayVsync(MonoObject* obj);
	};
}
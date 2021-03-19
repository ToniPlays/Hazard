#pragma once

#include "../../ScriptUtils.h"
#include "../../ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}	

namespace Hazard::Scripting::Bindings {

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
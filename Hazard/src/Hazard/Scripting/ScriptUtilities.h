#pragma once

#include "HazardScript.h"
#include "UID.h"

namespace Hazard 
{
	struct ObjectReference 
	{
#ifdef HZR_INCLUDE_MONO
		MonoObject* MonoObject = nullptr;
#endif
		uint32_t MonoObjectHandle = 0;
		UID ObjectUID = 0;
	};
}

#pragma once

#include "HazardScript.h"
#include "UID.h"

namespace Hazard 
{
	struct ObjectReference 
	{
		MonoObject* MonoObject;
		uint32_t MonoObjectHandle;
		UID ObjectUID = 0;
	};
}

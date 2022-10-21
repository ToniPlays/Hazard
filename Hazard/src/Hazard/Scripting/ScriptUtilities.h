#pragma once

#include "HazardScript.h"
#include "UID.h"

namespace Hazard 
{
	struct ObjectReference 
	{
		MonoObject* MonoObject = nullptr;
		uint32_t MonoObjectHandle = 0;
		UID ObjectUID = 0;
	};
}

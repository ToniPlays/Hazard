#pragma once

#include "HazardScript.h"
#include "Utility/UID.h"

namespace Hazard 
{
	struct ObjectReference 
	{
		//MonoObject* MonoObject = nullptr;
		uint32_t MonoObjectHandle = 0;
		UID ObjectUID = 0;
	};
}

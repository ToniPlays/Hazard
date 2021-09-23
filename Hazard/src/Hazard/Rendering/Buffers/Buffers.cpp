#pragma once

#include <hzrpch.h>
#include "Buffers.h"

namespace Hazard::Rendering {

	void* Buffer::Copy(void* data, uint32_t size)
	{
		void* copiedData = malloc(size);
		memcpy(copiedData, data, size);
		return copiedData;
	}
}

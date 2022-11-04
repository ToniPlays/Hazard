#pragma once

#include "Ref.h"

namespace HazardRenderer
{
	struct TopLevelAccelerationStructureCreateInfo
	{

	};

	class TopLevelAccelerationStructure : public RefCount
	{
	public:

		static Ref<TopLevelAccelerationStructure> Create(TopLevelAccelerationStructureCreateInfo* info);
	};
}
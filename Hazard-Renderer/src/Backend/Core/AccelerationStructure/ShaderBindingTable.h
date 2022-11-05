#pragma once

#include "Ref.h"
#include "Backend/Core/Pipeline/Pipeline.h"

namespace HazardRenderer
{
	struct ShaderBindingTableCreateInfo
	{
		Ref<Pipeline> pPipeline = nullptr;
	};

	class ShaderBindingTable : public RefCount
	{
	public:
		static Ref<ShaderBindingTable> Create(ShaderBindingTableCreateInfo* info);
	};
}
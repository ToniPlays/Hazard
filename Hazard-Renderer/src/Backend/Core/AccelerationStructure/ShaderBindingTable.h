#pragma once

#include "Ref.h"

namespace HazardRenderer
{
	struct ShaderBindingTableCreateInfo
	{

	};

	class ShaderBindingTable : public RefCount
	{
	public:

		static Ref<ShaderBindingTable> Create(ShaderBindingTableCreateInfo* info);
	};
}
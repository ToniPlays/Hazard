#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Scripting
{
	class Script
	{
	public:

		Script() = default;

		virtual const std::string& GetModuleName() = 0;
		virtual bool IsValid() const = 0;
		virtual uint32_t GetHandle() = 0;
	};
}
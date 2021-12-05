#pragma once

#include "Hazard/Core/Core.h"
#include "PublicField.h"
#include "Hazard/Assets/Asset.h"

namespace Hazard::Scripting
{
	class Script : public Asset
	{
	public:

		Script() = default;

		virtual const std::string& GetModuleName() = 0;
		virtual bool IsValid() const = 0;
		virtual uint32_t GetHandle() const = 0;

		virtual uint32_t GetPublicFieldCount() = 0;
		virtual PublicField& GetPublicField(uint32_t index) = 0;
	};
}
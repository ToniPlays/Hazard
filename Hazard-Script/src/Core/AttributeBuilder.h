#pragma once

#include "Mono/Core/Mono.h"
#include "Attribute.h"

namespace HazardScript
{
	class AttributeBuilder 
	{
	public:
		static Attribute* Create(const std::string className, MonoObject* obj);
	};
}
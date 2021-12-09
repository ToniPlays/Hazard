#pragma once
#include "Hazard/Core/Core.h"
#include "Mono/Mono.h"

namespace Hazard::Scripting::CSharp 
{
	class AttributeBuilder 
	{
	public:
		static Attribute* Create(const std::string className, MonoObject* obj);
	};
}
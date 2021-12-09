#pragma once
#include "Attribute.h"

namespace Hazard::Scripting
{
	class VisualFuncAttribute : public Attribute
	{
	public:
		ATTRIBUTE_CLASS_TYPE(VisualFunc)
	};
	class ExposeFuncAttribute : public Attribute
	{
	public:
		ATTRIBUTE_CLASS_TYPE(ExposeFunc)
	};
}
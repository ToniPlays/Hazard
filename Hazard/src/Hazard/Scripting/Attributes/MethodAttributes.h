#pragma once

#include "HazardScript.h"

namespace Hazard
{
	using namespace HazardScript;

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
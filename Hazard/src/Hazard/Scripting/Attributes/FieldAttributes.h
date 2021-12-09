#pragma once
#include "Attribute.h"

namespace Hazard::Scripting
{
	class ShowInPropertiesAttribute : public Attribute
	{
	public:
		ATTRIBUTE_CLASS_TYPE(ShowInProperties)
	};
	class HideInPropertiesAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(HideInProperties)
	};
	class SerializeFieldAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(SerializeField)
	};
	class SliderAttribute : public Attribute 
	{
	public:
		float Min, Max;

		ATTRIBUTE_CLASS_TYPE(Slider)
	};
	class RangeAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(Range)
	};
	class HeaderAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(Header)
	};
	class TooltipAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(Header)
	};
	class TextAreaAttribute : public Attribute 
	{
	public:
		ATTRIBUTE_CLASS_TYPE(TextArea)
	};
}
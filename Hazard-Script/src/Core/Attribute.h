#pragma once

#include "UtilityCore.h"

namespace HazardScript {

	enum class Progress {
		Done = 0,
		Awaiting = 1,
		InProgress = 2,
		Bug = 3
	};

	enum class AttributeType {
		None = 0,
		Author, 
		Todo,
		MenuBarItem,
		HideInProperties,
		ShowInProperties,
		SerializeField,
		Slider, 
		Range,
		Header,
		Tooltip,
		TextArea,
		VisualFunc,
		ExposeFunc
	};

	namespace Utils
	{
		static const char* ProgressToString(const Progress& progress);
	}

#define ATTRIBUTE_CLASS_TYPE(type)	static AttributeType GetStaticType() { return AttributeType::type; } \
									virtual AttributeType GetAttributeType() const override { return GetStaticType(); } \
									virtual const char* GetName() const override { return #type; }

	class Attribute {
	public:
		Attribute() = default;

		virtual AttributeType GetAttributeType() const { return AttributeType::None; };
		virtual const char* GetName() const { return "Attribute"; };
		virtual std::string ToString() const { return GetName(); }
	};
}
#pragma once

#include "UtilityCore.h"
#include "ManagedType.h"

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
		static const char* NativeTypeToString(const NativeType& type)
		{
			switch (type) {
			case NativeType::Void:			return "Void";
			case NativeType::Bool:			return "Bool";
			case NativeType::Float:			return "Float";
			case NativeType::Float2:		return "Float2";
			case NativeType::Float3:		return "Float3";
			case NativeType::Float4:		return "Float4";
			case NativeType::Double:		return "Double";
			case NativeType::Int8:			return "Int8";
			case NativeType::Int16:			return "Int16";
			case NativeType::Int32:			return "Int32";
			case NativeType::Int64:			return "Int64";
			case NativeType::UInt8:			return "UInt8";
			case NativeType::UInt16:		return "UInt16";
			case NativeType::UInt32:		return "UInt32";
			case NativeType::UInt64:		return "UInt64";
			case NativeType::String:		return "String";
			}
			return "Unknown";
		}
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
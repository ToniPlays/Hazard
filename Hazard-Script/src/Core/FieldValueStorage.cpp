#include "FieldValueStorage.h"
#include "Metadata/FieldMetadata.h"

namespace HazardScript 
{
	FieldValueStorage::FieldValueStorage(uint32_t index, FieldMetadata* field) : m_Index(index), m_Field(field)
	{
		m_Storage = GetDefaultValueForType(field->GetType());
	}
	ValueWrapper GetDefaultValueForType(const ManagedType& type)
	{

		switch (type.NativeType)
		{
		case NativeType::Bool:			return ValueWrapper(false);
		case NativeType::Float:			return ValueWrapper(0.0f);
		case NativeType::Float2:		return ValueWrapper(glm::vec2(0.0f));
		case NativeType::Float3:		return ValueWrapper(glm::vec3(0.0f));
		case NativeType::Float4:		return ValueWrapper(glm::vec4(0.0f));
		case NativeType::Double:		return ValueWrapper(0.0);
		case NativeType::Int8:			return ValueWrapper(int8_t(0));
		case NativeType::Int16:			return ValueWrapper(int16_t(0));
		case NativeType::Int32:			return ValueWrapper(int32_t(0));
		case NativeType::Int64:			return ValueWrapper(int64_t(0));
		case NativeType::UInt8:			return ValueWrapper(uint8_t(0));
		case NativeType::UInt16:		return ValueWrapper(uint16_t(0));
		case NativeType::UInt32:		return ValueWrapper(uint32_t(0));
		case NativeType::UInt64:		return ValueWrapper(uint64_t(0));
		case NativeType::String:		return ValueWrapper(std::string(""));
		}
		return ValueWrapper();
	}
}
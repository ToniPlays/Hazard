#pragma once

#include "Ref.h"
#include "UtilityCore.h"

namespace HazardScript
{
	static ValueWrapper GetDefaultValueForType(const ManagedType& type) {

		switch (type.NativeType) {
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

	class FieldValueStorageBase : public RefCount
	{

	};
	class FieldValueStorage : public FieldValueStorageBase
	{
	public:

		FieldValueStorage(ManagedType* type)
		{
			m_Storage = GetDefaultValueForType(*type);
		}

		bool HasValue() { return m_Storage.HasValue(); }

		template<typename T>
		T GetValue() {
			HZR_ASSERT(HasValue(), "Oops");

			if constexpr (std::is_same<T, ValueWrapper>::value)
				return m_Storage;
			else
				return m_Storage.Get<T>();
		}

		template<typename T>
		T GetValueOrDefault() {
			if (!HasValue()) return T();
			return GetValue<T>();
		}

		template<typename T>
		void SetValue(T value)
		{
			if (!HasValue()) 
			{
				m_Storage = ValueWrapper(&value, sizeof(T));
				return;
			}
			if constexpr (std::is_same<T, ValueWrapper>::value)
				m_Storage = value;
			else
				m_Storage.Set<T>(value);
		}

	private:
		ValueWrapper m_Storage;
	};
	class ArrayFieldValueStorage : public FieldValueStorageBase
	{
	public:
		uintptr_t GetLength() { return 0; }

		template<typename T>
		T GetValue(size_t index) { return T(); }

		template<typename T>
		void SetValue(size_t index, T value) {

		};
	};
}
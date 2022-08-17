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

		FieldValueStorage(const ManagedType& type)
		{
			m_Storage = GetDefaultValueForType(type);
		}

		bool HasValue() { return m_Storage.HasValue(); }

		template<typename T>
		T GetValue() {
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

		ArrayFieldValueStorage(const ManagedType& elementType) : m_ElementType(elementType) {
			std::cout << Utils::NativeTypeToString(elementType.NativeType) << std::endl;
		}

		uintptr_t GetLength() { return m_ArrayStorage.size(); }

		void Resize(uint32_t elements) 
		{
			m_ArrayStorage.resize(elements);

			for (auto& storage : m_ArrayStorage) 
			{
				if (storage.HasValue()) continue;
				storage = GetDefaultValueForType(m_ElementType);
			}
		}

		bool HasValue(uint32_t index) {
			return m_ArrayStorage[index].HasValue();
		}

		template<typename T>
		T GetValue(uint32_t index) 
		{
			if constexpr (std::is_same<T, ValueWrapper>::value)
				return m_ArrayStorage[index];
			else
				return m_ArrayStorage[index].Get<T>();
		}

		template<typename T>
		T GetValueOrDefault(uint32_t index) {
			if (!HasValue(index)) return T();
			return GetValue<T>(index);
		}

		template<typename T>
		void SetValue(size_t index, T value) {
			ValueWrapper& storage = m_ArrayStorage[index];
			if (!storage.HasValue())
			{
				storage = ValueWrapper(&value, sizeof(T));
				return;
			}
			if constexpr (std::is_same<T, ValueWrapper>::value)
				storage = value;
			else
				storage.Set<T>(value);
		}

	private:
		ManagedType m_ElementType;
		std::vector<ValueWrapper> m_ArrayStorage;
	};
}
#pragma once

#include "Ref.h"
#include "UtilityCore.h"
#include "ValueWrapper.h"
#include "Attribute.h"
#include "Metadata/ManagedType.h"
#include "FieldValueStorageBase.h"
#include "Metadata/FieldMetadata.h"
#include "MathCore.h"
#include "Mono/Core/MonoUtilities.h"


namespace HazardScript
{
	static ValueWrapper GetDefaultValueForType(const ManagedType& type);

#define DEFAULT_TYPE(Type)	template<>																					\
							Type GetStoredValue() { return m_Storage.Get<Type>(); }										\
							template<>																					\
							void SetStoredValue(Type value) { m_Storage.Set<Type>(value);}								\
							template<>																					\
							Type GetLiveValue(MonoObject* target) {														\
								if (m_Field->GetType().IsArray())														\
									return MonoArrayUtils::GetElementValue<Type>((MonoArray*)target, m_Index);			\
								return MonoFieldUtils::GetFieldValue<Type>(target, m_Field->GetMonoField());			\
							}																							\
							template<>																					\
							void SetLiveValue(MonoObject* target, Type value)											\
							{	if(m_Field->GetType().IsArray()) {														\
									MonoArrayUtils::SetElementValue<Type>((MonoArray*)target, m_Index, value);			\
									return;																				\
								}																						\
								MonoFieldUtils::SetFieldValue<Type>(target, m_Field->GetMonoField(), value);			\
							}																							\

	class FieldValueStorage : public FieldValueStorageBase
	{
	public:
		FieldValueStorage() {}
		FieldValueStorage(size_t index, FieldMetadata* field);

		bool HasValue() { return m_Storage.HasValue(); }
		bool Valid() { return m_Field != nullptr; }

		template<typename T>
		T GetValue(MonoObject* target) {
			return m_IsLiveValue ? GetLiveValue<T>(target) : GetStoredValue<T>();
		}
		template<typename T>
		void SetValue(MonoObject* target, T value) {
			m_IsLiveValue ? SetLiveValue<T>(target, value) : SetStoredValue<T>(value);
		}

		template<typename T>
		T GetStoredValue() {
			static_assert(false);
		}
		template<typename T>
		T GetLiveValue(MonoObject* target) {
			static_assert(false);
		}

		template<typename T>
		T GetStoredValueOrDefault()
		{
			if (!HasValue()) return T();
			return GetValue<T>();
		}

		template<typename T>
		void SetStoredValue(T value)
		{
			static_assert(false);
		}
		//Target is MonoClassField*, for array types it returns MonoArray*
		template<typename T>
		void SetLiveValue(MonoObject* target, T value)
		{
			static_assert(false);
		}

		DEFAULT_TYPE(bool);
		DEFAULT_TYPE(float);
		DEFAULT_TYPE(double);
		DEFAULT_TYPE(int8_t);
		DEFAULT_TYPE(int16_t);
		DEFAULT_TYPE(int32_t);
		DEFAULT_TYPE(int64_t);
		DEFAULT_TYPE(uint8_t);
		DEFAULT_TYPE(uint16_t);
		DEFAULT_TYPE(uint32_t);
		DEFAULT_TYPE(uint64_t);

		template<>
		std::string GetStoredValue()
		{
			return m_Storage.Get<std::string>();
		}
		template<>
		void SetStoredValue(std::string value)
		{
			m_Storage.Set<std::string>(value);
		}
		template<>
		std::string GetLiveValue(MonoObject* target) {
			if (m_Field->GetType().IsArray())
				return MonoArrayUtils::GetElementValue<std::string>((MonoArray*)target, m_Index);
			return MonoFieldUtils::GetFieldValue<std::string>(target, m_Field->GetMonoField());
		}
		template<>
		void SetLiveValue(MonoObject* target, std::string value)
		{
			if (m_Field->GetType().IsArray()) 
			{
				MonoArrayUtils::SetElementValue<std::string>((MonoArray*)target, m_Index, value);
				return;
			}
			MonoFieldUtils::SetFieldValue<std::string>(target, m_Field->GetMonoField(), value);
		}


	private:
		FieldMetadata* m_Field = nullptr;
		ValueWrapper m_Storage;
		size_t m_Index = 0;
	};
	class ArrayFieldValueStorage : public FieldValueStorageBase
	{
	public:

		ArrayFieldValueStorage(FieldMetadata* field) : m_Field(field) {}

		uintptr_t GetLength(MonoObject* target) { return IsLive() ? GetLiveLength(target) : m_ArrayStorage.size(); }

		uintptr_t GetLiveLength(MonoObject* target)
		{
			MonoArray* arr = (MonoArray*)target;
			if (arr == nullptr) return 0;
			uintptr_t len = mono_array_length(arr);
			return len;
		}

		void Resize(MonoObject* target, size_t elements)
		{
			m_ArrayStorage.resize(elements);
			for (size_t i = 0; i < elements; i++)
			{
				if (!m_ArrayStorage[i].Valid())
				{
					m_ArrayStorage[i] = FieldValueStorage(i, m_Field);
				}
			}

			MonoArray* arr = (MonoArray*)mono_field_get_value_object(Mono::GetDomain(), m_Field->GetMonoField(), target);
			if (arr == nullptr)
			{
				arr = mono_array_new(Mono::GetDomain(), m_Field->GetType().TypeClass->Class, elements);
				mono_field_set_value(target, m_Field->GetMonoField(), arr);
				return;
			}
			else
			{
				MonoArray* arr = (MonoArray*)mono_field_get_value_object(Mono::GetDomain(), m_Field->GetMonoField(), target);
				uintptr_t oldLenth = mono_array_length(arr);

				size_t copyLength = Math::Min<size_t>(m_ArrayStorage.size(), oldLenth);

				MonoClass* arrClass = mono_object_get_class((MonoObject*)arr);
				MonoClass* elementClass = m_Field->GetType().GetElementType().TypeClass->Class;
				int32_t elementSize = mono_array_element_size(arrClass);

				MonoArray* tmp = mono_array_new(Mono::GetDomain(), elementClass, m_ArrayStorage.size());

				char* src = mono_array_addr_with_size(arr, elementSize, 0);
				char* dst = mono_array_addr_with_size(tmp, elementSize, 0);

				memcpy(dst, src, copyLength * elementSize);
				mono_field_set_value(target, m_Field->GetMonoField(), tmp);
			}
		}

		bool HasValue(MonoObject* target, size_t index)
		{
			if (index >= GetLength(target)) return false;
			return m_ArrayStorage[index].HasValue();
		}

		template<typename T>
		T GetValue(MonoObject* target, size_t index)
		{
			if (IsLive()) {
				return MonoArrayUtils::GetElementValue<T>((MonoArray*)target, index);
			}

			if constexpr (std::is_same<T, ValueWrapper>::value)
				return m_ArrayStorage[index];
			else
				return m_ArrayStorage[index].GetValue<T>(target);
		}

		template<typename T>
		T GetValueOrDefault(MonoObject* target, size_t index)
		{
			if (IsLive()) {
				return MonoArrayUtils::GetElementValue<T>((MonoArray*)target, index);
			}
			if (!HasValue(target, index)) return T();
			return GetValue<T>(target, index);
		}

		template<typename T>
		void SetStoredValue(size_t index, T value)
		{
			m_ArrayStorage[index].SetStoredValue<T>(value);
		}
		template<typename T>
		void SetLiveValue(MonoObject* target, size_t index, T value)
		{
			MonoArrayUtils::SetElementValue<T>((MonoArray*)target, index, value);
		}

	private:
		FieldMetadata* m_Field;
		std::vector<FieldValueStorage> m_ArrayStorage;
	};
}
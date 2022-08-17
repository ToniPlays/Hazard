#pragma once

#include "Ref.h"
#include "UtilityCore.h"
#include "ValueWrapper.h"
#include "Attribute.h"
#include "Metadata/ManagedType.h"
#include "FieldValueStorageBase.h"
#include "Metadata/FieldMetadata.h"

namespace HazardScript
{
	static ValueWrapper GetDefaultValueForType(const ManagedType& type);

#define DEFAULT_TYPE(Type)	template<>																\
							void SetStoredValue(Type value) { m_Storage.Set<Type>(value);}			\
							template<>																\
							Type GetStoredValue() { return m_Storage.Get<Type>(); }					\
							template<>																\
							void SetLiveValue(MonoObject* object, Type value)						\
							{																		\
								mono_field_set_value(object, m_Field.GetMonoField(), &value);		\
							}																		\



	class FieldValueStorage : public FieldValueStorageBase
	{
	public:
		FieldValueStorage() {}
		FieldValueStorage(FieldMetadata& field);

		bool HasValue() { return m_Storage.HasValue(); }

		template<typename T>
		T GetValue(MonoObject* object) {
			return m_IsLiveValue ? GetLiveValue<T>(object) : GetStoredValue<T>();
		}
		template<typename T>
		void SetValue(MonoObject* object, T value) {
			m_IsLiveValue ? SetLiveValue<T>(object, value) : SetStoredValue<T>(value);
		}

		template<typename T>
		T GetStoredValue() {
			static_assert(false);
		}
		template<typename T>
		T GetLiveValue(MonoObject* object) {
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
		template<typename T>
		void SetLiveValue(MonoObject* object, T value)
		{
			static_assert(false);
		}

		DEFAULT_TYPE(float);
		template<>
		float GetLiveValue(MonoObject* object) {
			float value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(double);
		template<>
		double GetLiveValue(MonoObject* object) {
			double value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(int8_t);
		template<>
		int8_t GetLiveValue(MonoObject* object) {
			int8_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(int16_t);
		template<>
		int16_t GetLiveValue(MonoObject* object) {
			int16_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(int32_t);
		template<>
		int32_t GetLiveValue(MonoObject* object) {
			int32_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(int64_t);
		template<>
		int64_t GetLiveValue(MonoObject* object) {
			int64_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(uint8_t);
		template<>
		uint8_t GetLiveValue(MonoObject* object) {
			uint8_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(uint16_t);
		template<>
		uint16_t GetLiveValue(MonoObject* object) {
			uint16_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(uint32_t);
		template<>
		uint32_t GetLiveValue(MonoObject* object) {
			uint32_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}
		DEFAULT_TYPE(uint64_t);
		template<>
		uint64_t GetLiveValue(MonoObject* object) {
			uint64_t value;
			mono_field_get_value(object, m_Field.GetMonoField(), &value);
			return value;
		}

		template<>
		void SetStoredValue(std::string value)
		{
			m_Storage.Set<std::string>(value);
		}
		template<>
		std::string GetStoredValue()
		{
			return m_Storage.Get<std::string>();
		}
		template<>
		std::string GetLiveValue(MonoObject* object) {
			MonoString* string;
			mono_field_get_value(object, m_Field.GetMonoField(), &string);
			return Mono::MonoStringToString(string);
		}
		template<>
		void SetLiveValue(MonoObject* object, std::string value)
		{
			std::cout << "Setting live string " << value << std::endl;
			MonoString* string = Mono::StringToMonoString(value);
			mono_field_set_value(object, m_Field.GetMonoField(), string);
		}


	private:
		FieldMetadata m_Field;
		ValueWrapper m_Storage;
	};
	class ArrayFieldValueStorage : public FieldValueStorageBase
	{
	public:

		ArrayFieldValueStorage(const ManagedType& elementType) : m_ElementType(elementType) {
			std::cout << Utils::NativeTypeToString(elementType.NativeType) << std::endl;
		}

		uint32_t GetLength() { return m_ArrayStorage.size(); }

		virtual void TransitionToLive() { };

		void Resize(uint32_t elements)
		{
			m_ArrayStorage.resize(elements);
		}

		bool HasValue(uint32_t index) {
			return m_ArrayStorage[index].HasValue();
		}

		template<typename T>
		T GetValue(MonoObject* object, uint32_t index)
		{
			if constexpr (std::is_same<T, ValueWrapper>::value)
				return m_ArrayStorage[index];
			else
				return m_ArrayStorage[index].GetValue<T>(object);
		}

		template<typename T>
		T GetValueOrDefault(MonoObject* object, uint32_t index) {
			if (!HasValue(index)) return T();
			return GetValue<T>(object, index);
		}

		template<typename T>
		void SetStoredValue(size_t index, T value)
		{
			m_ArrayStorage[index].SetStoredValue<T>(value);
		}
		template<typename T>
		void SetLiveValue(MonoObject* object, size_t index, T value)
		{
			m_ArrayStorage[index].SetLiveValue<T>(object, value);
		}

	private:
		ManagedType m_ElementType;
		std::vector<FieldValueStorage> m_ArrayStorage;
	};
}
#pragma once

#include "Mono/Core/Mono.h"
#include "Core/ValueWrapper.h"
#include "Core/ScriptCache.h"

namespace HazardScript
{
	class MonoUtils {
	public:
		template<typename T>
		static T Unbox(MonoObject* object) {
			return *(T*)mono_object_unbox(object);
		}
		template<typename T>
		static MonoObject* Box(MonoClass* type, T value) 
		{
			return mono_value_box(Mono::GetDomain(), type, &value);
		}
		static MonoClass* GetTargetClass(MonoObject* object)
		{
			return mono_object_get_class(object);
		}
	};


	class MonoFieldUtils {
	public:
		template<typename T>
		static T GetFieldValue(MonoObject* target, MonoClassField* field) 
		{
			T result = {};
			mono_field_get_value(target, field, &result);
			return result;
		}
		template<>
		static bool GetFieldValue(MonoObject* target, MonoClassField* field)
		{
			MonoObject* result = mono_field_get_value_object(mono_domain_get(), field, target);
			return (bool)MonoUtils::Unbox<MonoBoolean>(result);
		}
		template<>
		static MonoObject* GetFieldValue(MonoObject* target, MonoClassField* field)
		{
			return mono_field_get_value_object(mono_domain_get(), field, target);
		}

		template<>
		static std::string GetFieldValue(MonoObject* target, MonoClassField* field)
		{
			MonoObject* result;
			MonoClass* klass = mono_object_get_class(target);
			mono_field_get_value(target, field, &result);
			return Mono::MonoObjectToString(result);
		}

		template<typename T>
		static T GetFieldValue(MonoObject* target, const std::string& name)
		{
			MonoClass* klass = mono_object_get_class(target);
			return GetFieldValue<T>(target, mono_class_get_field_from_name(klass, name.c_str()));
		}

		template<>
		static std::string GetFieldValue(MonoObject* object, const std::string& name)
		{
			MonoClass* klass = mono_object_get_class(object);
			return GetFieldValue<std::string>(object, mono_class_get_field_from_name(klass, name.c_str()));
		}

		template<typename T>
		static void SetFieldValue(MonoObject* target, MonoClassField* field, T value) 
		{
			mono_field_set_value(target, field, &value);
		}
		template<>
		static void SetFieldValue(MonoObject* target, MonoClassField* field, MonoObject* value) 
		{
			mono_field_set_value(target, field, value);
		}

		template<>
		static void SetFieldValue(MonoObject* target, MonoClassField* field, std::string value)
		{
			MonoString* string = Mono::StringToMonoString(value);
			mono_field_set_value(target, field, string);
		}
		template<>
		static void SetFieldValue(MonoObject* target, MonoClassField* field, bool value)
		{
			mono_field_set_value(target, field, &value);
		}
	};

	class MonoArrayUtils {
	public:
		template<typename T>
		static T GetElementValue(MonoArray* arrayObject, size_t index)
		{
			uintptr_t length = mono_array_length(arrayObject);
			if (index >= length) __debugbreak();

			MonoClass* arrayClass = mono_object_get_class((MonoObject*)arrayObject);
			MonoClass* elementClass = mono_class_get_element_class(arrayClass);
			int32_t size = mono_array_element_size(arrayClass);
			ManagedType type = ManagedType::FromClass(elementClass);

			if (type.IsReference()) {
				MonoObject* elem = mono_array_get(arrayObject, MonoObject*, index);
				if constexpr (std::is_same<T, MonoObject*>::value)
					return elem;
				else {
					if (elem == nullptr) return T();
					return MonoUtils::Unbox<T>(elem);
				}
			}
			char* src = mono_array_addr_with_size(arrayObject, size, index);
			return ValueWrapper(src, sizeof(T)).Get<T>();
		}
		template<typename T>
		static void SetElementValue(MonoArray* arrayObject, size_t index, T value)
		{
			if constexpr (std::is_same<T, MonoObject*>::value)
				SetValueInternal(arrayObject, index, value);
			else if constexpr (std::is_same<T, ValueWrapper>::value)
				SetValueInternal(arrayObject, index, value);
			else SetElementValue(arrayObject, index, ValueWrapper(&value, sizeof(T)));
		}

		static void* GetArrayValuePtr(MonoArray* monoArray) {
			uintptr_t size = mono_array_length(monoArray);
			return mono_array_addr_with_size(monoArray, (int)size, 0);
		}

	private:
		static void SetValueInternal(MonoArray* arrayObject, size_t index, const ValueWrapper& value);
		static void SetValueInternal(MonoArray* arrayObject, size_t index, MonoObject* value);
	};
}
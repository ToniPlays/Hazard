
#include <hzrpch.h>
#include "MonoUtilities.h"

namespace HazardScript
{

	//glm::vec2 -----------------------------------
	using namespace Hazard;
	template<>
	glm::vec2 FieldValueStorage::GetStoredValue() 
	{
		return m_Storage.Get<glm::vec2>();
	}

	template<>
	void FieldValueStorage::SetStoredValue(glm::vec2 value) 
	{
		m_Storage.Set(value);
	}

	template<>
	glm::vec2 FieldValueStorage::GetLiveValue(MonoObject* object) 
	{
		glm::vec2 result;
		mono_field_get_value(object, m_Field->GetMonoField(), &result);
		return result;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec2 value)
	{
		mono_field_set_value(object, m_Field->GetMonoField(), &value);
	}

	//glm::vec3 -----------------------------------

	using namespace Hazard;
	template<>
	glm::vec3 FieldValueStorage::GetStoredValue() 
	{
		return m_Storage.Get<glm::vec3>();
	}

	template<>
	void FieldValueStorage::SetStoredValue(glm::vec3 value) 
	{
		m_Storage.Set(value);
	}

	template<>
	glm::vec3 FieldValueStorage::GetLiveValue(MonoObject* object) 
	{
		glm::vec3 result;
		mono_field_get_value(object, m_Field->GetMonoField(), &result);
		return result;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec3 value)
	{
		mono_field_set_value(object, m_Field->GetMonoField(), &value);
	}

	//glm::vec4 -----------------------------------

	using namespace Hazard;
	template<>
	glm::vec4 FieldValueStorage::GetStoredValue() 
	{
		return m_Storage.Get<glm::vec4>();
	}

	template<>
	void FieldValueStorage::SetStoredValue(glm::vec4 value) 
	{
		m_Storage.Set(value);
	}

	template<>
	glm::vec4 FieldValueStorage::GetLiveValue(MonoObject* object) 
	{
		glm::vec4 result;
		mono_field_get_value(object, m_Field->GetMonoField(), &result);
		return result;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec4 value)
	{
		mono_field_set_value(object, m_Field->GetMonoField(), &value);
	}
	//Object reference -----------------------------------

	using namespace Hazard;
	template<>
	ObjectReference FieldValueStorage::GetStoredValue()
	{
		if (!m_Storage.HasValue()) return ObjectReference();
		return m_Storage.Get<ObjectReference>();
	}

	template<>
	void FieldValueStorage::SetStoredValue(ObjectReference value)
	{
		if (!HasValue())
		{
			m_Storage = ValueWrapper(&value, sizeof(ObjectReference));
			return;
		}
		m_Storage.Set(value);
	}

	template<>
	ObjectReference FieldValueStorage::GetLiveValue(MonoObject* object)
	{
		ObjectReference result;
		return result;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, ObjectReference value)
	{
		if (value.MonoObject == nullptr) {
			value.MonoObjectHandle = Mono::InstantiateHandle(m_Field->GetType().TypeClass->Class);
			value.MonoObject = mono_gchandle_get_target(value.MonoObjectHandle);
			SetStoredValue(value);
		}

		mono_field_set_value(object, m_Field->GetMonoField(), value.MonoObject);
		MonoMethod* method = mono_class_get_method_from_name(m_Field->GetType().TypeClass->Class, ".ctor", 1);
		if (method != nullptr) {
			void* params[] = { &value.ObjectUID };
			mono_runtime_invoke(method, value.MonoObject, params, nullptr);
		}
	}
}
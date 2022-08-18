
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
		return MonoFieldUtils::GetFieldValue<glm::vec2>(object, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec2 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(object, m_Field->GetMonoField(), value);
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
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		return MonoFieldUtils::GetFieldValue<glm::vec3>(object, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec3 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(object, m_Field->GetMonoField(), value);
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
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		return MonoFieldUtils::GetFieldValue<glm::vec4>(object, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec4 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(object, m_Field->GetMonoField(), value);
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
		ObjectReference stored = GetStoredValue<ObjectReference>();
		ObjectReference ref = {};
		if (!m_Field->GetType().IsArray()) {

			ref.MonoObject = MonoFieldUtils::GetFieldValue<MonoObject*>(object, m_Field->GetMonoField());
			ref.MonoObjectHandle = stored.MonoObject == ref.MonoObject ? stored.MonoObjectHandle : 0;
		}
		else {

			ref.MonoObject = MonoArrayUtils::GetElementValue<MonoObject*>((MonoArray*)object, m_Index);
			ref.MonoObjectHandle = stored.MonoObject == ref.MonoObject ? stored.MonoObjectHandle : 0;

		}
		ManagedClass* entityManaged = ScriptCache::GetManagedClassByName("Hazard.Entity");

		if (m_Field->GetType().IsSubClassOf(entityManaged) && ref.MonoObject != nullptr)
		{
			//Get EntityManaged ID
			MonoProperty* prop = mono_class_get_property_from_name(m_Field->GetType().TypeClass->Class, "ID");
			MonoMethod* getter = mono_property_get_get_method(prop);
			MonoObject* result = mono_runtime_invoke(getter, ref.MonoObject, NULL, NULL);
			ref.ObjectUID = MonoUtils::Unbox<uint64_t>(result);
		}
		else
		{

		}
		return ref;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, ObjectReference value)
	{
		if (value.MonoObject == nullptr) {
			value.MonoObjectHandle = Mono::InstantiateHandle(m_Field->GetType().TypeClass->Class);
			value.MonoObject = mono_gchandle_get_target(value.MonoObjectHandle);
			SetStoredValue(value);
		}

		if (!m_Field->GetType().IsArray())
		{
			MonoFieldUtils::SetFieldValue(object, m_Field->GetMonoField(), value.MonoObject);
		}
		else
		{
			MonoArray* arr = (MonoArray*)object;
			MonoArrayUtils::SetElementValue(arr, m_Index, value.MonoObject);
		}

		MonoMethod* method = mono_class_get_method_from_name(m_Field->GetType().TypeClass->Class, ".ctor", 1);

		if (method != nullptr) {
			void* params[] = { &value.ObjectUID };
			mono_runtime_invoke(method, value.MonoObject, params, nullptr);
		}
	}
}
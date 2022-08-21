
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
	glm::vec2 FieldValueStorage::GetLiveValue(MonoObject* target)
	{
		return MonoFieldUtils::GetFieldValue<glm::vec2>(target, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec2 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(target, m_Field->GetMonoField(), value);
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
	glm::vec3 FieldValueStorage::GetLiveValue(MonoObject* target)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		return MonoFieldUtils::GetFieldValue<glm::vec3>(target, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec3 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(target, m_Field->GetMonoField(), value);
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
	glm::vec4 FieldValueStorage::GetLiveValue(MonoObject* target)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		return MonoFieldUtils::GetFieldValue<glm::vec4>(target, m_Field->GetMonoField());
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec4 value)
	{
		HZR_ASSERT(!m_Field->GetType().IsArray(), "Oops");
		MonoFieldUtils::SetFieldValue(target, m_Field->GetMonoField(), value);
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
	ObjectReference FieldValueStorage::GetLiveValue(MonoObject* target)
	{
		ObjectReference stored = GetStoredValue<ObjectReference>();
		ObjectReference ref = {};

		if (!m_Field->GetType().IsArray())
			ref.MonoObject = MonoFieldUtils::GetFieldValue<MonoObject*>(target, m_Field->GetMonoField());
		else
			ref.MonoObject = MonoArrayUtils::GetElementValue<MonoObject*>((MonoArray*)target, m_Index);

		ref.MonoObjectHandle = stored.MonoObject == ref.MonoObject ? stored.MonoObjectHandle : 0;
		ManagedClass* entityManaged = ScriptCache::GetManagedClassByName("Hazard.Reference");

		if (m_Field->GetType().IsSubClassOf(entityManaged) && ref.MonoObject != nullptr)
		{
			MonoObject* result = Mono::GetPropertyObject(ref.MonoObject, m_Field->GetType().TypeClass->Class, "ID");
			ref.ObjectUID = MonoUtils::Unbox<uint64_t>(result);
		}
		else
		{

		}
		return ref;
	}

	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, ObjectReference value)
	{
		if (value.MonoObject == nullptr)
		{
			value.MonoObjectHandle = Mono::InstantiateHandle(m_Field->GetType().TypeClass->Class);
			value.MonoObject = mono_gchandle_get_target(value.MonoObjectHandle);

			SetStoredValue(value);
		}
		else if (value.ObjectUID == 0) 
		{
			value.MonoObject = nullptr;
			mono_gchandle_free(value.MonoObjectHandle);
			SetStoredValue(value);
			return;
		}

		ManagedClass* entityManaged = ScriptCache::GetManagedClassByName("Hazard.Reference");
		MonoMethod* method = mono_class_get_method_from_name(entityManaged->Class, ".ctor", 1);
		if (method != nullptr) {

			uint64_t id = value.ObjectUID;
			void* params[] = { &id };
			Mono::RuntimeInvoke(value.MonoObject, method, params);
		}

		if (!m_Field->GetType().IsArray())
			MonoFieldUtils::SetFieldValue(target, m_Field->GetMonoField(), value.MonoObject);
		else
			MonoArrayUtils::SetElementValue((MonoArray*)target, m_Index, value.MonoObject);
	}
}
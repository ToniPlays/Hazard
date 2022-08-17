
#pragma once
#include "ScriptUtilities.h"

namespace HazardScript
{
	using namespace Hazard;

	template<>
	glm::vec2 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec2 value);
	template<>
	glm::vec2 FieldValueStorage::GetLiveValue(MonoObject* object);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec2 value);

	template<>
	glm::vec3 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec3 value);
	template<>
	glm::vec3 FieldValueStorage::GetLiveValue(MonoObject* object);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec3 value);

	template<>
	glm::vec4 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec4 value);
	template<>
	glm::vec4 FieldValueStorage::GetLiveValue(MonoObject* object);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, glm::vec4 value);

	template<>
	ObjectReference FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(ObjectReference value);
	template<>
	ObjectReference FieldValueStorage::GetLiveValue(MonoObject* object);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* object, ObjectReference value);
}
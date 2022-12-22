
#pragma once
#include "ScriptUtilities.h"
#include "Hazard/Assets/AssetEnums.h"

namespace Hazard::Utils
{
	AssetType ScriptClassToAsset(const std::string_view& type);
}

namespace HazardScript
{
#ifdef HZR_INCLUDE_MONO
	using namespace Hazard;
	template<>
	glm::vec2 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec2 value);
	template<>
	glm::vec2 FieldValueStorage::GetLiveValue(MonoObject* target);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec2 value);

	template<>
	glm::vec3 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec3 value);
	template<>
	glm::vec3 FieldValueStorage::GetLiveValue(MonoObject* target);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec3 value);

	template<>
	glm::vec4 FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(glm::vec4 value);
	template<>
	glm::vec4 FieldValueStorage::GetLiveValue(MonoObject* target);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, glm::vec4 value);

	template<>
	ObjectReference FieldValueStorage::GetStoredValue();
	template<>
	void FieldValueStorage::SetStoredValue(ObjectReference value);
	template<>
	ObjectReference FieldValueStorage::GetLiveValue(MonoObject* target);
	template<>
	void FieldValueStorage::SetLiveValue(MonoObject* target, ObjectReference value);
#endif
}

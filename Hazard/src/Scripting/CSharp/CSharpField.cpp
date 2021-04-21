#pragma once

#include <hzrpch.h>
#include "CSharpField.h"

namespace Hazard::Scripting::CSharp {

	CSharpField::CSharpField(FieldType type) : PublicField(type)
	{
		buffer = ScriptUtils::AllocateBuffer(type);
	}
	void CSharpField::CopyStoredToRuntimeValue()
	{
		Mono::SetFieldValue(entityInstance->GetInstance(), monoClassField, buffer);
	}
	bool CSharpField::RuntimeAvailable()
	{
		return entityInstance->handle != 0;
	}
	void CSharpField::GetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(value, buffer, size);
	}
	void CSharpField::SetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(buffer, value, size);
	}
	void CSharpField::GetRuntimeValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		Mono::GetFieldValue(entityInstance->GetInstance(), monoClassField, value);
	}
	void CSharpField::SetRuntimeValueInternal(void* value) const
	{
		Mono::SetFieldValue(entityInstance->GetInstance(), monoClassField, value);
	}
}

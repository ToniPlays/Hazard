#pragma once

#include <hzrpch.h>
#include "CSharpField.h"

namespace Hazard::Scripting::CSharp {

	CSharpField::CSharpField(const std::string& name, FieldType type) : PublicField(name, type)
	{
		buffer = ScriptUtils::AllocateBuffer(type);
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

	}
	void CSharpField::SetRuntimeValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);

	}
}

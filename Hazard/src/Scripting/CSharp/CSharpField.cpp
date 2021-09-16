#pragma once

#include <hzrpch.h>
#include "CSharpField.h"

namespace Hazard::Scripting::CSharp {

	CSharpField::CSharpField(FieldType type) : PublicField(type)
	{
		m_Buffer = ScriptUtils::AllocateBuffer(type);
	}
	void CSharpField::CopyStoredToRuntimeValue()
	{
		Mono::SetFieldValue(m_EntityInstance->GetInstance(), m_MonoClassField, m_Buffer);
	}
	bool CSharpField::RuntimeAvailable()
	{
		return m_EntityInstance->handle != 0;
	}
	void CSharpField::GetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(m_Type);
		memcpy(value, m_Buffer, size);
	}
	void CSharpField::SetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(m_Type);
		memcpy(m_Buffer, value, size);
	}
	void CSharpField::GetRuntimeValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(m_Type);
		Mono::GetFieldValue(m_EntityInstance->GetInstance(), m_MonoClassField, &value);
	}
	void CSharpField::SetRuntimeValueInternal(void* value) const
	{
		Mono::SetFieldValue(m_EntityInstance->GetInstance(), m_MonoClassField, &value);
	}
}

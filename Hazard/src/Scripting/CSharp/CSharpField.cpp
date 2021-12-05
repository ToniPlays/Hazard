#pragma once

#include <hzrpch.h>
#include "CSharpField.h"
#include "CSharpScript.h"

namespace Hazard::Scripting::CSharp {

	CSharpField::CSharpField(const std::string& name, FieldType type) : m_Name(name), m_Type(type)
	{
		m_Buffer = ScriptUtils::AllocateBuffer(type);
	}
	CSharpField::CSharpField(const std::string& name, const std::string& customType) : m_Name(name), m_CustomType(customType), m_Type(FieldType::Custom)
	{
		HZR_CORE_ASSERT(false, "No support for custom type yet");
	}
	void CSharpField::CopyStoredToRuntimeValue()
	{
		SetRuntimeValueInternal((void*)m_Buffer);
	}
	bool CSharpField::RuntimeAvailable()
	{
		return m_Script->GetHandle() != 0;
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
		Mono::GetFieldValue(Mono::ObjectFromHandle(m_Script->GetHandle()), m_MonoClassField, value);
	}
	void CSharpField::SetRuntimeValueInternal(void* value) const
	{
		float val = *(float*)value;


		Mono::SetFieldValue(Mono::ObjectFromHandle(m_Script->GetHandle()), m_MonoClassField, value);
	}
}

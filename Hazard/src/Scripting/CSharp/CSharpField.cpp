#pragma once

#include <hzrpch.h>
#include "CSharpField.h"
#include "CSharpScript.h"
#include "Mono/Mono.h"

namespace Hazard::Scripting::CSharp {

	CSharpField::CSharpField(const ScriptFieldMetadata& metadata, CSharpScript* parent) : m_Metadata(metadata)
	{
		m_Buffer = ScriptUtils::AllocateBuffer(metadata.Type);
		m_Script = parent;
		m_MonoClassField = Mono::GetField(Mono::GetMonoClass(parent->GetModuleName().c_str()), metadata.Name);
	}
	CSharpField::~CSharpField()
	{
		delete m_Buffer;
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
		uint32_t size = ScriptUtils::GetFieldSize(m_Metadata.Type);
		memcpy(value, m_Buffer, size);
	}
	void CSharpField::SetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(m_Metadata.Type);
		memcpy(m_Buffer, value, size);
	}
	void CSharpField::GetRuntimeValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(m_Metadata.Type);
		Mono::GetFieldValue(Mono::ObjectFromHandle(m_Script->GetHandle()), m_MonoClassField, value);
	}
	void CSharpField::SetRuntimeValueInternal(void* value) const
	{
		Mono::SetFieldValue(Mono::ObjectFromHandle(m_Script->GetHandle()), m_MonoClassField, value);
	}
}

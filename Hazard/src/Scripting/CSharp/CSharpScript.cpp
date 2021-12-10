#pragma once

#include "hzrpch.h"
#include "CSharpScript.h"
#include "CSharpField.h"
#include "Mono/Mono.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::Scripting::CSharp {

	CSharpScript::CSharpScript(const ScriptMetadata& data) : m_Metadata(data)
	{
		m_MonoClass = Mono::GetMonoClass(data.ModuleName.c_str());
		m_ScriptHandle = Mono::InstantiateHandle(m_MonoClass);

		InitClassMethods();
		UpdateFields();

		auto* obj = Mono::ObjectFromHandle(m_ScriptHandle);

		for (auto& [index, field] : m_Fields)
		{
			switch (field->GetType()) {
			case FieldType::Float:	field->SetStoredValue(Mono::GetFieldValue<float>(obj, field->GetName()));
			case FieldType::Float2:	field->SetStoredValue(Mono::GetFieldValue<glm::vec2>(obj, field->GetName()));
			case FieldType::Float3:	field->SetStoredValue(Mono::GetFieldValue<glm::vec3>(obj, field->GetName()));
			case FieldType::Float4:	field->SetStoredValue(Mono::GetFieldValue<glm::vec4>(obj, field->GetName()));
			case FieldType::Int:	field->SetStoredValue(Mono::GetFieldValue<int>(obj, field->GetName()));
			case FieldType::UInt:	field->SetStoredValue(Mono::GetFieldValue<uint32_t>(obj, field->GetName()));
			}
		}
	}
	CSharpScript::~CSharpScript()
	{
		Mono::FreeHandle(m_ScriptHandle);

		for (auto& [index, field] : m_Fields) {
			delete field;
		}
		m_Fields.clear();
	}
	void CSharpScript::InitClassMethods()
	{
		//TODO constructo should not be like thiss
		Constructor = Mono::GetCoreMethod("Hazard.Entity:.ctor(ulong)");
		OnCreated = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnCreate()");
		OnStart = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnStart()");
		OnUpdate = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnUpdate(single)");
		OnLateUpdate = Mono::GetAppMethod(m_Metadata.ModuleName + ":LateUpdate()");
		OnFixedUpdate = Mono::GetAppMethod(m_Metadata.ModuleName + ":FixedUpdate()");
		OnColliderEnter2D = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnColliderEnter2D(Collider2D)");
		OnTriggerEnter2D = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnTriggerEnter2D(Collider2D)");
		OnColliderExit2D = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnColliderExit2D(Collider2D)");
		OnTriggerExit2D = Mono::GetAppMethod(m_Metadata.ModuleName + ":OnTriggerExit2D(Collider2D)");
	}
	void CSharpScript::UpdateFields()
	{
		uint32_t i = 0;
		for (auto& field : m_Metadata.Fields) 
		{
			m_Fields[i] = new CSharpField(field, this);
			i++;
		}
	}
	void CSharpScript::SetRuntimeValues()
	{
		for (auto& [index, field] : m_Fields) 
		{
			field->CopyStoredToRuntimeValue();
		}
	}
}

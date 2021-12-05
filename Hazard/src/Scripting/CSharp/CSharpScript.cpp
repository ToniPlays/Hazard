#pragma once

#include "hzrpch.h"
#include "CSharpScript.h"
#include "CSharpField.h"
#include "Mono/Mono.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::Scripting::CSharp {

	CSharpScript::CSharpScript(const std::string& moduleName) : m_ModuleName(moduleName)
	{
		m_MonoClass = Mono::GetMonoClass(moduleName.c_str());
		m_ScriptHandle = Mono::InstantiateHandle(m_MonoClass);
	}
	CSharpScript::~CSharpScript()
	{
		Mono::FreeHandle(m_ScriptHandle);
	}
	void CSharpScript::InitClassMethods()
	{
		Constructor = Mono::GetCoreMethod("Hazard.Entity:.ctor(ulong)");
		OnCreated = Mono::GetAppMethod(m_ModuleName + ":OnCreate()");
		OnStart = Mono::GetAppMethod(m_ModuleName + ":OnStart()");
		OnUpdate = Mono::GetAppMethod(m_ModuleName + ":OnUpdate(single)");
		OnLateUpdate = Mono::GetAppMethod(m_ModuleName + ":LateUpdate()");
		OnFixedUpdate = Mono::GetAppMethod(m_ModuleName + ":FixedUpdate()");
		OnColliderEnter2D = Mono::GetAppMethod(m_ModuleName + ":OnColliderEnter2D(Collider2D)");
		OnTriggerEnter2D = Mono::GetAppMethod(m_ModuleName + ":OnTriggerEnter2D(Collider2D)");
		OnColliderExit2D = Mono::GetAppMethod(m_ModuleName + ":OnColliderExit2D(Collider2D)");
		OnTriggerExit2D = Mono::GetAppMethod(m_ModuleName + ":OnTriggerExit2D(Collider2D)");
	}
	void CSharpScript::UpdatePublicFields()
	{
		m_PublicFields.clear();

		MonoClassField* iter;
		void* ptr = nullptr;

		while (iter = Mono::GetMonoField(m_MonoClass, &ptr)) {

			std::string name = Mono::GetFieldName(iter);
			if (Mono::GetVisibility(iter) != FieldVisibility::Public) 
				continue;

			FieldType fieldType = ScriptUtils::GetFieldType(iter);
			
			if (fieldType != FieldType::Custom) {
				CSharpField* field = new CSharpField(name, fieldType);
				field->SetParentScript(this);
				field->SetField(iter);
				float value = 0;
				Mono::GetFieldValue(Mono::ObjectFromHandle(m_ScriptHandle), iter, &value);
				field->SetStoredValue<float>(value);
				m_PublicFields.push_back(field);
			}
		}

	}
	void CSharpScript::SetRuntimeValues()
	{
		for (auto& field : m_PublicFields) 
		{
			field->CopyStoredToRuntimeValue();
		}
	}
}

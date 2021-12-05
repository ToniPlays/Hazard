#pragma once

#include "hzrpch.h"
#include "CSharpScript.h"
#include "Mono/Mono.h"

namespace Hazard::Scripting::CSharp {

	CSharpScript::CSharpScript(const std::string& moduleName) : m_ModuleName(moduleName)
	{
		m_MonoClass = Mono::GetMonoClass(moduleName.c_str());
		m_Handle = Mono::InstantiateHandle(m_MonoClass);
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
}

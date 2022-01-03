
#include "hzrpch.h"
#include "ScriptEngineManager.h"
#include "ScriptCommand.h"
#include "Scripting/ScriptEngines.h"
#include "Hazard/Physics/PhysicsCommand.h"

#include "ScriptResourceManager.h"

namespace Hazard::Scripting {

	ScriptEngineManager::ScriptEngineManager(ScriptEngineCreateInfo* info) : Module("ScriptManager") 
	{
		HZR_PROFILE_FUNCTION();
#ifdef HZR_INCLUDE_MONO
		//m_ScriptEngines[ScriptType::CSharpScript] = new CSharp::CSharpEngine(info);
#endif

		ScriptCommand::Init(*this);
		ScriptResourceManager::Init();
		SetActive(info->Enable);

	}
	ScriptEngineManager::~ScriptEngineManager()
	{

	}
	void ScriptEngineManager::Close()
	{
		HZR_PROFILE_FUNCTION();
		for (auto [type, engine] : m_ScriptEngines) {
			engine->OnApplicationClose();
		}
	}
	void ScriptEngineManager::Update() 
	{
		//TODO: Remove
		Physics::PhysicsCommand::Step(Time::s_DeltaTime);
		ECS::WorldCommand::UpdatePhysics();

		for (auto[type, engine] : m_ScriptEngines) {
			engine->UpdateEntities();
		}
	}
	void ScriptEngineManager::Instantiate(ScriptType type, uint32_t handle, const std::string& moduleName)
	{
		m_ScriptEngines[type]->Instantiate(handle, moduleName);
	}
	void ScriptEngineManager::OnRuntimeEnd()
	{
		ScriptResourceManager::Clear();
		for (auto [type, engine] : m_ScriptEngines) {
			engine->OnEndRuntime();
		}
	}
	void ScriptEngineManager::InitEntity(ScriptType type, uint32_t handle, const std::string& moduleName)
	{
		if (m_ScriptEngines.find(type) == m_ScriptEngines.end()) return;
		m_ScriptEngines[type]->InitializeEntity(handle, moduleName);
	}
	void ScriptEngineManager::ClearEntity(ScriptType type, uint32_t handle, const std::string& moduleName)
	{
		if (m_ScriptEngines.find(type) == m_ScriptEngines.end()) return;
		m_ScriptEngines[type]->ClearEntity(handle, moduleName);
	}
	std::unordered_map<uint32_t, ScriptField*> ScriptEngineManager::GetFields(ScriptType type, uint32_t handle, const std::string& moduleName)
	{
		ScriptRegistry registry = m_ScriptEngines[type]->GetRegistry();
		std::unordered_map<uint32_t, ScriptField*> result;

		if (!registry.HasInstance(handle)) 
			return result;

		for (auto& script : registry.GetInstanceData(handle).Scripts)
		{
			if (script->GetModuleName() == moduleName) {
				return script->GetFields();
			}
		}
		return result;
	}
	void ScriptEngineManager::ReloadAll()
	{
		HZR_CORE_INFO("Reloading assemblies");

		for (auto [type, engine] : m_ScriptEngines) {
			engine->Reload();
		}

		ScriptCommand::InitAllEntities();
	}
	bool ScriptEngineManager::ModuleExists(ScriptType type, const char* moduleName) {
		if (m_ScriptEngines.find(type) == m_ScriptEngines.end()) return false;
		return m_ScriptEngines[type]->ModuleExists(moduleName);
	}
}


#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Component.h"
#include "Hazard/Entity/WorldHandler.h"
//TODO: Remove usage
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::Scripting {

	void(*ScriptCommand::debugCallback)(Severity, const std::string&);
	ScriptEngineManager* ScriptCommand::s_Manager;

	ScriptMetadata ScriptCommand::GetMetadata(const std::string& moduleName) 
	{
		return s_Manager->GetEngine(ScriptType::CSharpScript).GetRegistry().GetScriptMetadata(moduleName);
	}

	void ScriptCommand::Init(ScriptEngineManager& manager)
	{
		Physics::PhysicsCommand::Init();
		s_Manager = &manager;

		auto fn = [&](Physics::Contact2DData data) 
		{
			using namespace Physics;
			uint32_t p = data.CollidedTo;
			void* param[] = { &p };

			if (data.Type == ContactType::Collision) 
			{
				for (auto& [type, engine] : GetEngines())
				{
					engine->OnCollided(data.Collider, param, data.State == CollisionState::ContactBegin);
				}
			}
			else 
			{
				for (auto& [type, engine] : GetEngines())
				{
					engine->OnTrigger(data.Collider, param, data.State == CollisionState::ContactBegin);
				}
			}
		};

		Physics::PhysicsCommand::AddContact2DCallback(fn);
	}
	bool ScriptCommand::ReloadAssemblies() 
	{
		if (!s_Manager) return false;
		s_Manager->ReloadAll();
		return true;
	}

	void ScriptCommand::OnBeginRuntime()
	{
		using namespace ECS;
		Ref<World> current = Application::GetModule<WorldHandler>().GetCurrentWorld();
		auto view = current->GetWorldRegistry().view<ScriptComponent>();

		for (auto entity : view) {

			Entity e = current->GetEntity(entity);
			auto& c = e.GetComponent<ScriptComponent>();

			if (ModuleExists(ScriptType::CSharpScript, c.ModuleName.c_str())) {
				s_Manager->Instantiate(ScriptType::CSharpScript, (uint32_t)entity, c.ModuleName);
			}
		}
	}
	void ScriptCommand::OnEndRuntime()
	{
		s_Manager->OnRuntimeEnd();
	}
	void ScriptCommand::InitAllEntities()
	{	
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();

		for (auto& e : world->GetEntitiesWith<ECS::ScriptComponent>()) 
		{
			ECS::Entity entity = { e, world.Raw() };
			auto& cc = entity.GetComponent<ECS::ScriptComponent>();
			if (!s_Manager->ModuleExists(ScriptType::CSharpScript, cc.ModuleName.c_str()))
				continue;
			
			s_Manager->InitEntity(ScriptType::CSharpScript, (uint32_t)e, cc.ModuleName);
		}
	}
	void ScriptCommand::InitEntity(uint32_t handle, ECS::ScriptComponent& component)
	{
		s_Manager->InitEntity(ScriptType::CSharpScript, handle, component.ModuleName);
	}
	void ScriptCommand::ClearEntity(uint32_t handle, ECS::ScriptComponent& component)
	{
		s_Manager->ClearEntity(ScriptType::CSharpScript, handle, component.ModuleName);
	}
	void ScriptCommand::InitEntity(uint32_t handle, ECS::VisualScriptComponent& component)
	{
		s_Manager->InitEntity(ScriptType::VisualScript, handle, component.Filename);
	}
	void ScriptCommand::ClearEntity(uint32_t handle, ECS::VisualScriptComponent& component)
	{
		s_Manager->ClearEntity(ScriptType::VisualScript, handle, component.Filename);
	}
	void ScriptCommand::SendDebugMessage(Severity severity, const std::string& message)
	{
		debugCallback(severity, message);
	}
	ScriptEngine& ScriptCommand::GetEngine(ScriptType type)
	{
		return s_Manager->GetEngine(type);
	}
}

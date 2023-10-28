#pragma once

#include "UtilityCore.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/Entity.h"

#define RegisterComponent(T, Assembly) {																				\
		Coral::TypeId coralType = Assembly->GetTypeByName("Hazard." #T).GetTypeId();									\
		if (coralType)																									\
		{																												\
			hasComponentFuncs[coralType] = [](uint64_t id) {															\
				return ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).HasComponent<T>();				\
			};																											\
			createComponentFuncs[coralType] = [](uint64_t id) {															\
				return ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).GetComponent<T>();				\
			};																											\
		}																												\
	}																													\

namespace Hazard 
{
	std::unordered_map<Coral::TypeId, std::function<bool(uint64_t)>> hasComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<ComponentBase&(uint64_t)>> getComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<void(uint64_t)>> createComponentFuncs;

	using namespace HazardScript;

	static uint64_t Entity_InstantiateOrigin_Native(Coral::NativeString name)
	{
		//TODO
		return ScriptEngine::GetTargetWorldForEntity(0)->CreateEntity(name).GetUID();
	}
	static uint64_t Entity_InstantiateAt_Native(Coral::NativeString name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		//TODO
		//Entity e = CreateEntity(name.ToString());

		//auto& tc = e.GetTransform();
		//tc.SetTranslation(position);
		//tc.SetRotation(rotation);
		//tc.SetScale(scale);

		return 0;
		//return e.GetUID();
	}
	static bool Entity_HasComponent_Native(uint64_t id, Coral::TypeId type)
	{
		return hasComponentFuncs[type](id);
	}
	static void Entity_CreateComponent_Native(uint64_t id, Coral::TypeId type)
	{
		createComponentFuncs[type](id);
	}
	
	static void Entity_Destroy_Native(uint64_t id)
	{
		Entity e = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		if (e.HasComponent<ScriptComponent>()) 
		{
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle)
				sc.m_Handle->TryInvoke("OnDestroy()", nullptr);
		}
		Application::GetModule<WorldHandler>().GetCurrentWorld()->DestroyEntity(e);
	}
	static bool Entity_IsUpdated_Native(uint64_t id)
	{
		return ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).ReceivesUpdate();
	}
	static void Entity_SetUpdate_Native(uint64_t id, bool visible)
	{
		ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).SetReceiveUpdate(visible);
	}
	static bool Entity_IsVisible_Native(uint64_t id) 
	{
		return ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).IsVisible();
	}
	static void Entity_SetVisible_Native(uint64_t id, bool visible)
	{
		ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).SetVisible(visible);
	}
	static bool Component_IsActive_Native(uint64_t id, Coral::TypeId type)
	{
		return getComponentFuncs[type](id).Active;
	}
	static void Component_SetActive_Native(uint64_t id, bool active, Coral::TypeId type)
	{
		ComponentBase& base = getComponentFuncs[type](id);
		base.Active = active;
	}
}

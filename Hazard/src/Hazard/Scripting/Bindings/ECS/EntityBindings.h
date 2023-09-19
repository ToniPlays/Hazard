#pragma once

#define GET_ENTITY(id) Application::GetModule<WorldHandler>().GetCurrentWorld()->GetEntityFromUID(id)

#include "UtilityCore.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/Entity.h"

#define RegisterComponent(Type, Assembly) {																						\
		Coral::TypeId coralType = Assembly->GetTypeByName("Hazard." #Type ", " + std::string(assembly->GetQualifiedName()));	\
		if (coralType)																											\
		{																														\
			hasComponentFuncs[coralType] = [](uint64_t entityID) { return GET_ENTITY(entityID).HasComponent<Type>(); };			\
			createComponentFuncs[coralType] = [](uint64_t entityID) { GET_ENTITY(entityID).AddComponent<Type>(); };				\
		}																														\
	}																															\

namespace Hazard 
{
	std::unordered_map<Coral::TypeId, std::function<bool(uint64_t)>> hasComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<ComponentBase&(uint64_t)>> getComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<void(uint64_t)>> createComponentFuncs;

	using namespace HazardScript;

	static uint64_t Entity_InstantiateOrigin_Native(Coral::NativeString name)
	{
		return Application::GetModule<WorldHandler>().GetCurrentWorld()->CreateEntity(name.ToString()).GetUID();
	}
	static uint64_t Entity_InstantiateAt_Native(Coral::NativeString name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		Entity e = Application::GetModule<WorldHandler>().GetCurrentWorld()->CreateEntity(name.ToString());

		auto& tc = e.GetTransform();
		tc.SetTranslation(position);
		tc.SetRotation(rotation);
		tc.SetScale(scale);

		return e.GetUID();
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
		Entity e = GET_ENTITY(id);
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
		return GET_ENTITY(id).ReceivesUpdate();
	}
	static void Entity_SetUpdate_Native(uint64_t id, bool visible)
	{
		GET_ENTITY(id).SetReceiveUpdate(visible);
	}
	static bool Entity_IsVisible_Native(uint64_t id) 
	{
		return GET_ENTITY(id).IsVisible();
	}
	static void Entity_SetVisible_Native(uint64_t id, bool visible)
	{
		GET_ENTITY(id).SetVisible(visible);
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

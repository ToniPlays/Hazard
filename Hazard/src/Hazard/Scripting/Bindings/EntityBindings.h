#pragma once

#define GET_ENTITY(id) handler->GetCurrentWorld()->GetEntityFromUID(id)

#include "UtilityCore.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/Entity.h"
#include "Mono/Core/Mono.h"

#define RegisterComponent(Type, Image)	{																												\
		MonoType* monoType = Mono::MonoTypeFromReflectionName("Hazard." #Type, Image);																	\
		if (monoType)																																	\
		{																																				\
			hasComponentFuncs[monoType] = [](uint64_t entityID) { return GET_ENTITY(entityID).HasComponent<Type>(); };									\
			getComponentFuncs[monoType] = [](uint64_t entityID) -> ComponentBase& { return GET_ENTITY(entityID).GetComponent<Type>(); };				\
			createComponentFuncs[monoType] = [](uint64_t entityID) { GET_ENTITY(entityID).AddComponent<Type>(); };										\
		}																																				\
	}

namespace Hazard 
{
	WorldHandler* handler;

	std::unordered_map<MonoType*, std::function<bool(uint64_t)>> hasComponentFuncs;
	std::unordered_map<MonoType*, std::function<ComponentBase&(uint64_t)>> getComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(uint64_t)>> createComponentFuncs;

	using namespace HazardScript;

	static uint64_t Entity_InstantiateOrigin_Native(MonoObject* name)
	{
		std::string entityName = Mono::MonoObjectToString(name);
		return handler->GetCurrentWorld()->CreateEntity(entityName).GetUID();
	}
	static uint64_t Entity_InstantiateAt_Native(MonoObject* name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		std::string entityName = Mono::MonoObjectToString(name);
		Entity e = handler->GetCurrentWorld()->CreateEntity(entityName);

		auto& tc = e.GetTransform();
		tc.SetTranslation(position);
		tc.SetRotation(rotation);
		tc.SetScale(scale);

		return e.GetUID();
	}
	static bool Entity_HasComponent_Native(uint64_t id, void* type)
	{
		HZR_ASSERT(handler, "woop");
		MonoType* compType = mono_reflection_type_get_type((MonoReflectionType*)type);
		return hasComponentFuncs[compType](id);
	}
	static void Entity_CreateComponent_Native(uint64_t id, void* type)
	{
		MonoType* compType = mono_reflection_type_get_type((MonoReflectionType*)type);
		createComponentFuncs[compType](id);
	}
	static void Entity_Destroy_Native(uint64_t id)
	{
		Entity e = GET_ENTITY(id);
		if (e.HasComponent<ScriptComponent>()) 
		{
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->TryInvoke("OnDestroy()", nullptr);
			}
		}
		handler->GetCurrentWorld()->DestroyEntity(e);
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
	static bool Component_IsActive_Native(uint64_t id, MonoType* type)
	{
		MonoType* compType = mono_reflection_type_get_type((MonoReflectionType*)type);
		return getComponentFuncs[compType](id).Active;
	}
	static void Component_SetActive_Native(uint64_t id, bool active, MonoType* type)
	{
		MonoType* compType = mono_reflection_type_get_type((MonoReflectionType*)type);
		ComponentBase& base = getComponentFuncs[compType](id);

		base.Active = active;
	}
}

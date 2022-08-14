#pragma once

#include "UtilityCore.h"
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/Entity.h"
#include "Mono/Core/Mono.h"

#define GET_ENTITY(id) handler->GetCurrentWorld()->GetEntityFromUID(id)
#define RegisterComponent(Type, Image)	{																												\
		MonoType* monoType = Mono::MonoTypeFromReflectionName("Hazard." #Type, Image);																\
		if (monoType)																																	\
		{																																				\
			hasComponentFuncs[monoType] = [](uint64_t entityID) { return GET_ENTITY(entityID).HasComponent<Type>(); };									\
			createComponentFuncs[monoType] = [](uint64_t entityID) { GET_ENTITY(entityID).AddComponent<Type>(); };																				\
		}																																				\
	}

namespace Hazard 
{
	WorldHandler* handler;

	std::unordered_map<MonoType*, std::function<bool(uint64_t)>> hasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(uint64_t)>> createComponentFuncs;

	using namespace HazardScript;

	static uint64_t Entity_InstantiateOrigin_Native(MonoString* name)
	{
		std::string entityName = Mono::MonoStringToString(name);
		return handler->GetCurrentWorld()->CreateEntity(entityName).GetUID();
	}
	static uint64_t Entity_InstantiateAt_Native(MonoString* name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		std::string entityName = Mono::MonoStringToString(name);
		Entity e = handler->GetCurrentWorld()->CreateEntity(entityName);

		auto& tc = e.GetTransform();
		tc.Translation = position;
		tc.Rotation = rotation;
		tc.Scale = scale;

		return e.GetUID();
	}

	static bool Entity_HasComponent_Native(uint64_t id, void* type)
	{
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
		if (e.HasComponent<ScriptComponent>()) {
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->TryInvoke("OnDestroy()", nullptr);
			}
		}
		handler->GetCurrentWorld()->DestroyEntity(e);
	}
}
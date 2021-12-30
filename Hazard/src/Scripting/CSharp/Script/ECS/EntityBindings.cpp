#pragma once
#include <hzrpch.h>
#include "EntityBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"
#include "Hazard/Entity/Component.h"
#include "../../Mono/Mono.h"

namespace Hazard::Scripting::CSharp::Bindings {


	std::unordered_map<MonoType*, std::function<bool(uint32_t)>> hasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(uint32_t)>> createComponentFuncs;

	#define RegisterComponent(Type)	{																							\
		MonoType* monoType = Mono::TypeFromReflectionName("Hazard." #Type);														\
		if (monoType)																											\
		{																														\
			hasComponentFuncs[monoType] = [](uint32_t entityID) { return ScriptCommand::EntityHasComponent<Type>(entityID); };	\
			createComponentFuncs[monoType] = [](uint32_t entityID) { ScriptCommand::EntityAddComponent<Type>(entityID); };		\
		}																														\
	}																															\

	BindMap EntityBindings::GetBindings()
	{
		using namespace ECS;

		hasComponentFuncs.clear();
		createComponentFuncs.clear();

		RegisterComponent(TagComponent);
		RegisterComponent(TransformComponent);
		RegisterComponent(AudioSourceComponent);

		BindMap result;
		result.reserve(4);
		result.emplace_back("Hazard.Entity::Entity_HasComponent_Native", (void*) HasComponent);
		result.emplace_back("Hazard.Entity::Entity_CreateComponent_Native", (void*) CreateComponent);
		result.emplace_back("Hazard.Entity::Entity_GetActive_Native", (void*) IsActive);
		result.emplace_back("Hazard.Entity::Entity_SetActive_Native", (void*) SetActive);
		return result;
	}
	bool EntityBindings::IsActive(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::TagComponent>(id).Visible;
	}
	void EntityBindings::SetActive(uint32_t id, bool active)
	{
		ScriptCommand::EntityGetComponent<ECS::TagComponent>(id).Visible = active;
	}
	bool EntityBindings::HasComponent(uint32_t id, void* type)
	{
		MonoType* compType = Mono::ReflectionToType(type);
		return hasComponentFuncs[compType](id);
	}
	bool EntityBindings::CreateComponent(uint32_t id, void* type)
	{
		//TODO: Fix
		return false;
	}
}
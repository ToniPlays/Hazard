#pragma once
#include <hzrpch.h>
#include "EntityBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"
#include "Hazard/Entity/Component.h"



namespace Hazard::Scripting::CSharp::Bindings {


	std::unordered_map<MonoType*, std::function<bool(uint32_t)>> hasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(uint32_t)>> createComponentFuncs;

	#define RegisterComponent(Type)	{																							\
																																\
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

		BindMap result;
		result.emplace_back("Hazard.Entity::Entity_HasComponent_Native", HasComponent);
		return result;
	}
	bool EntityBindings::HasComponent(uint32_t id, void* type)
	{
		MonoType* compType = Mono::ReflectionToType(type);
		HZR_CORE_INFO("Does have component yes? {0}", Mono::GetTypeName(compType));

		return hasComponentFuncs[compType](id);
	}
}
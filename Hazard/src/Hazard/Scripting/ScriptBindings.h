#pragma once

#include "ScriptEngine.h"
#include "Hazard/Core/Application.h"
#include "HazardScript.h"
#include "Hazard/ECS/WorldHandler.h"

#define BIND_ICALL(x) engine->RegisterInternalCall("Hazard.InternalCalls::"#x, (void*)x)
#define GET_ENTITY(id) handler->GetCurrentWorld()->GetEntityFromUID(id)
#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() "StackTrace: " __FUNCTION__ ":" LINE(__LINE__) 

using namespace HazardScript;

namespace Hazard
{
	std::unordered_map<MonoType*, std::function<bool(uint64_t)>> hasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(uint64_t)>> createComponentFuncs;
	WorldHandler* handler;

#define RegisterComponent(Type, Image)	{																												\
		MonoType* monoType = Mono::MonoTypeFromReflectionName("Hazard." #Type, Image);																	\
		if (monoType)																																	\
		{																																				\
			hasComponentFuncs[monoType] = [](uint64_t entityID) { return handler->GetCurrentWorld()->GetEntityFromUID(entityID).HasComponent<Type>(); };												\
			createComponentFuncs[monoType] = [](uint64_t entityID) {  };																				\
		}																																				\
	}
#pragma region Debug
	using namespace HazardScript;
	static void Debug_Log_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Debug, message, STACK_TRACE() });
	}
	static void Debug_Info_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Info, message, STACK_TRACE() });
	}
	static void Debug_Warn_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Warning, message, STACK_TRACE() });
	}
	static void Debug_Error_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Error, message, STACK_TRACE() });
	}
	static void Debug_Critical_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Critical, message, STACK_TRACE() });
	}
	static void Debug_Trace_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Trace, message, STACK_TRACE() });
	}
#pragma endregion

	static bool Entity_HasComponent_Native(uint64_t id, void* type)
	{
		MonoType* compType = mono_reflection_type_get_type((MonoReflectionType*)type);
		return hasComponentFuncs[compType](id);
	}
	static void Entity_Destroy_Native(uint64_t id)
	{
		Entity e = handler->GetCurrentWorld()->GetEntityFromUID(id);
		if (e.HasComponent<ScriptComponent>()) {
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->TryInvoke("OnDestroy()", nullptr);
			}
		}
		handler->GetCurrentWorld()->DestroyEntity(e);
	}

	static MonoString* Tag_GetName_Native(uint64_t id) {
		auto& tag = GET_ENTITY(id).GetComponent<TagComponent>();
		return Mono::StringToMonoString(tag.Tag);
	}

	static void Transform_GetPosition_Native(uint64_t id, glm::vec3* position) 
	{
		*position = GET_ENTITY(id).GetComponent<TransformComponent>().Translation;
	}

	static void Transform_SetPosition_Native(uint64_t id, glm::vec3 position)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Translation = position;
	}

	static void Transform_GetRotation_Native(uint64_t id, glm::vec3* rotation)
	{
		*rotation = GET_ENTITY(id).GetComponent<TransformComponent>().Rotation;
	}
	static void Transform_SetRotation_Native(uint64_t id, glm::vec3 rotation)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Rotation = rotation;
	}
	static void Transform_GetScale_Native(uint64_t id, glm::vec3* scale)
	{
		*scale = GET_ENTITY(id).GetComponent<TransformComponent>().Scale;
	}

	static void Transform_SetScale_Native(uint64_t id, glm::vec3 scale)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Scale = scale;
	}

	class InternalCall : public IScriptGlue {
	public:
		virtual void Register(ScriptEngine* engine) {
			handler = &Application::GetModule<WorldHandler>();
			MonoImage* image = HazardScript::HazardScriptEngine::GetMonoData().CoreAssembly.GetImage();

			RegisterComponent(TagComponent, image);
			RegisterComponent(TransformComponent, image);

			BIND_ICALL(Debug_Log_Native);
			BIND_ICALL(Debug_Info_Native);
			BIND_ICALL(Debug_Warn_Native);
			BIND_ICALL(Debug_Error_Native);
			BIND_ICALL(Debug_Critical_Native);
			BIND_ICALL(Debug_Trace_Native);

			BIND_ICALL(Entity_HasComponent_Native);
			BIND_ICALL(Entity_Destroy_Native);

			BIND_ICALL(Tag_GetName_Native);

			BIND_ICALL(Transform_GetPosition_Native);
			BIND_ICALL(Transform_SetPosition_Native);
			BIND_ICALL(Transform_GetRotation_Native);
			BIND_ICALL(Transform_SetRotation_Native);
			BIND_ICALL(Transform_GetScale_Native);
			BIND_ICALL(Transform_SetScale_Native);
		}
		virtual void OnAssemblyLoaded(HazardScript::ScriptAssembly* assembly) {};
	};

}
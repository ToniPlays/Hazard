#pragma once

#include "ScriptEngine.h"
#include "Hazard/Core/Application.h"
#include "HazardScript.h"
#include "Hazard/ECS/WorldHandler.h"

#include "AllBindings.h"

#define BIND_ICALL(x) engine->RegisterInternalCall("Hazard.InternalCalls::"#x, (void*)x)

using namespace HazardScript;

namespace Hazard
{
	class InternalCall : public IScriptGlue {
	public:
		virtual void Register(ScriptEngine* engine)
		{
			handler = &Application::GetModule<WorldHandler>();
			MonoImage* image = HazardScript::HazardScriptEngine::GetMonoData().CoreAssembly.GetImage();

			RegisterComponent(TagComponent, image);
			RegisterComponent(TransformComponent, image);
			RegisterComponent(ScriptComponent, image);
			RegisterComponent(MeshComponent, image);
			RegisterComponent(SpriteRendererComponent, image);

			RegisterComponent(CameraComponent, image);

			RegisterComponent(SkyLightComponent, image);
			RegisterComponent(DirectionalLightComponent, image);
			RegisterComponent(PointLightComponent, image);

			RegisterComponent(Rigidbody2DComponent, image);
			RegisterComponent(BoxCollider2DComponent, image);
			RegisterComponent(CircleCollider2DComponent, image);

			RegisterComponent(RigidbodyComponent, image);
			RegisterComponent(BoxColliderComponent, image);
			RegisterComponent(SphereColliderComponent, image);

			BIND_ICALL(CameraComponent_GetFOV_Native);
			BIND_ICALL(CameraComponent_SetFOV_Native);

			BIND_ICALL(SpriteRendererComponent_GetColor_Native);
			BIND_ICALL(SpriteRendererComponent_SetColor_Native);

			BIND_ICALL(TagComponent_GetName_Native);
			BIND_ICALL(TagComponent_SetName_Native);

			BIND_ICALL(TransformComponent_GetPosition_Native);
			BIND_ICALL(TransformComponent_SetPosition_Native);
			BIND_ICALL(TransformComponent_GetRotation_Native);
			BIND_ICALL(TransformComponent_SetRotation_Native);
			BIND_ICALL(TransformComponent_GetScale_Native);
			BIND_ICALL(TransformComponent_SetScale_Native);

			BIND_ICALL(SpriteRendererComponent_GetColor_Native);
			BIND_ICALL(SpriteRendererComponent_SetColor_Native);

			BIND_ICALL(Entity_IsVisible_Native);
			BIND_ICALL(Entity_SetVisible_Native);
			BIND_ICALL(Entity_IsUpdated_Native);
			BIND_ICALL(Entity_SetUpdate_Native);

			BIND_ICALL(Component_IsActive_Native);
			BIND_ICALL(Component_SetActive_Native);

			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateAt_Native);
			BIND_ICALL(Entity_HasComponent_Native);
			BIND_ICALL(Entity_CreateComponent_Native);
			BIND_ICALL(Entity_Destroy_Native);

			BIND_ICALL(Debug_Log_Native);
			BIND_ICALL(Debug_Info_Native);
			BIND_ICALL(Debug_Warn_Native);
			BIND_ICALL(Debug_Error_Native);
			BIND_ICALL(Debug_Critical_Native);
			BIND_ICALL(Debug_Trace_Native);
		}
		virtual void OnAssemblyLoaded(HazardScript::ScriptAssembly* assembly) {};
	};

}
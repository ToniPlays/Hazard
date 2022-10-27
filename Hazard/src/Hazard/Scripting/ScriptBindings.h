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
			HZR_PROFILE_FUNCTION();
			handler = &Application::GetModule<WorldHandler>();
			MonoImage* image = HazardScript::HazardScriptEngine::GetMonoData().CoreAssembly->GetImage();

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

			//Entity -------------------------------------------
			BIND_ICALL(Entity_IsVisible_Native);
			BIND_ICALL(Entity_SetVisible_Native);
			BIND_ICALL(Entity_IsUpdated_Native);
			BIND_ICALL(Entity_SetUpdate_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateOrigin_Native);
			BIND_ICALL(Entity_InstantiateAt_Native);
			BIND_ICALL(Entity_HasComponent_Native);
			BIND_ICALL(Entity_CreateComponent_Native);
			BIND_ICALL(Entity_Destroy_Native);

			//Component ----------------------------------------
			BIND_ICALL(Component_IsActive_Native);
			BIND_ICALL(Component_SetActive_Native);

			//Tag component ------------------------------------
			BIND_ICALL(TagComponent_GetName_Native);
			BIND_ICALL(TagComponent_SetName_Native);

			//Transform component ------------------------------
			BIND_ICALL(TransformComponent_GetPosition_Native);
			BIND_ICALL(TransformComponent_SetPosition_Native);
			BIND_ICALL(TransformComponent_GetRotation_Native);
			BIND_ICALL(TransformComponent_SetRotation_Native);
			BIND_ICALL(TransformComponent_GetScale_Native);
			BIND_ICALL(TransformComponent_SetScale_Native);

			//Camera component ---------------------------------
			BIND_ICALL(CameraComponent_GetFOV_Native);
			BIND_ICALL(CameraComponent_SetFOV_Native);
			BIND_ICALL(CameraComponent_GetOrthoSize_Native);
			BIND_ICALL(CameraComponent_SetOrthoSize_Native);
			BIND_ICALL(CameraComponent_GetClipping_Native);
			BIND_ICALL(CameraComponent_SetClipping_Native);
			BIND_ICALL(CameraComponent_GetProjection_Native);
			BIND_ICALL(CameraComponent_SetProjection_Native);

			//Sprite renderer component ------------------------
			BIND_ICALL(SpriteRendererComponent_GetColor_Native);
			BIND_ICALL(SpriteRendererComponent_SetColor_Native);
			BIND_ICALL(SpriteRendererComponent_GetSprite_Native);
			BIND_ICALL(SpriteRendererComponent_SetSprite_Native);

			//Mesh renderer component --------------------------
			BIND_ICALL(MeshComponent_GetMesh_Native);
			BIND_ICALL(MeshComponent_SetMesh_Native);

			//SkyLightComponent component ----------------------
			BIND_ICALL(SkyLightComponent_GetIntensity_Native);
			BIND_ICALL(SkyLightComponent_SetIntensity_Native);
			BIND_ICALL(SkyLightComponent_GetEnvironmentMap_Native);
			BIND_ICALL(SkyLightComponent_SetEnvironmentMap_Native);

			//Asset	--------------------------------------------
			BIND_ICALL(Asset_GetName_Native);

			//WorldManager--------------------------------------
			BIND_ICALL(WorldManager_LoadWorld_Native);
			BIND_ICALL(WorldManager_LoadWorldFromString_Native);


			//Debug --------------------------------------------
			BIND_ICALL(Debug_Log_Native);
			BIND_ICALL(Debug_Info_Native);
			BIND_ICALL(Debug_Warn_Native);
			BIND_ICALL(Debug_Error_Native);
			BIND_ICALL(Debug_Critical_Native);
			BIND_ICALL(Debug_Trace_Native);


			//Math ---------------------------------------------
			BIND_ICALL(Math_RandomRange_Native);
			BIND_ICALL(Math_Pow_Native);
			BIND_ICALL(Math_Sqrt_Native);
			BIND_ICALL(Math_Radians_Native);

			//Time ---------------------------------------------
			BIND_ICALL(Time_GetDelta_Native);
			BIND_ICALL(Time_GetUnscaledDelta_Native);
			BIND_ICALL(Time_GetSinceStart_Native);
			BIND_ICALL(Time_GetDeltaScale_Native);

			//Rendering
			BIND_ICALL(VertexBuffer_Create_Native);
			BIND_ICALL(VertexBuffer_Destroy_Native);
			BIND_ICALL(IndexBuffer_Create_Native);
			BIND_ICALL(IndexBuffer_Destroy_Native);

			BIND_ICALL(Mesh_Create_Native);
			BIND_ICALL(Mesh_Destroy_Native);

			BIND_ICALL(EnvironmentMap_Create_Native);
			BIND_ICALL(EnvironmentMap_Destroy_Native);

			BIND_ICALL(EnvironmentMap_GetCubemapTexture_Native);
			BIND_ICALL(EnvironmentMap_SetCubemapTexture_Native);

			BIND_ICALL(EnvironmentMap_GenerateRadiance_Native);
			BIND_ICALL(EnvironmentMap_GenerateIrradiance_Native);
			BIND_ICALL(EnvironmentMap_GeneratePrefilter_Native);
		}
		virtual void OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly) {};
	};

}
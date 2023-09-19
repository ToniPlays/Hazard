#pragma once

#include "ScriptEngine.h"
#include "Hazard/Core/Application.h"
#include "HazardScript.h"
#include "Hazard/ECS/WorldHandler.h"

#include "AllBindings.h"

using namespace HazardScript;

namespace Hazard
{
	class InternalCalls : public IScriptGlue
	{
	public:
		virtual void Register(Ref<ScriptAssembly> assembly)
		{
			HZR_PROFILE_FUNCTION();

			RegisterComponent(TagComponent, assembly);
			RegisterComponent(TransformComponent, assembly);
			RegisterComponent(ScriptComponent, assembly);
			RegisterComponent(MeshComponent, assembly);
			RegisterComponent(SpriteRendererComponent, assembly);

			RegisterComponent(CameraComponent, assembly);

			RegisterComponent(SkyLightComponent, assembly);
			RegisterComponent(DirectionalLightComponent, assembly);
			RegisterComponent(PointLightComponent, assembly);

			RegisterComponent(Rigidbody2DComponent, assembly);
			RegisterComponent(BoxCollider2DComponent, assembly);
			RegisterComponent(CircleCollider2DComponent, assembly);

			RegisterComponent(RigidbodyComponent, assembly);
			RegisterComponent(BoxColliderComponent, assembly);
			RegisterComponent(SphereColliderComponent, assembly);

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

			//Window -------------------------------------------
			BIND_ICALL(Display_IsFullscreen_Native);
			BIND_ICALL(Display_SetFullscreen_Native);
			BIND_ICALL(Display_IsVsync_Native);
			BIND_ICALL(Display_SetVsync_Native);
			BIND_ICALL(Display_GetResolution_Native);
			BIND_ICALL(Display_GetResolutions_Native);
			BIND_ICALL(Display_SetResolution_Native);

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
			BIND_ICALL(Time_SetDeltaScale_Native);

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
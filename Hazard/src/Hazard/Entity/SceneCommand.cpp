#pragma once

#include <hzrpch.h>
#include "SceneCommand.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Core/Application.h"
#include "Component.h"
#include "Loader/SceneHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"

namespace Hazard::ECS {

	using namespace Hazard::Rendering;

	template<typename T>
	void SceneCommand::OnScriptAttached(Entity& entity, T& script)
	{
		static_assert(false);
	}
	
	template<>
	void SceneCommand::OnScriptAttached(Entity& entity, ScriptComponent& script)  {
		Scripting::ScriptCommand::InitEntity(entity, script);
	}
	template<>
	void SceneCommand::OnScriptAttached(Entity& entity, VisualScriptComponent& script)  {}
	template<typename T>
	void SceneCommand::OnScriptDetached(Entity& entity, T& script)
	{
		static_assert(false);
	}
	template<>
	void SceneCommand::OnScriptDetached(Entity& entity, ScriptComponent& script)
	{
		Scripting::ScriptCommand::ClearEntity(entity, script);
	}
	template<>
	void SceneCommand::OnScriptDetached(Entity& entity, VisualScriptComponent& script)
	{
		Scripting::ScriptCommand::ClearEntity(entity, script);
	}

	Entity SceneCommand::GetEntity(uint32_t id)
	{
		return GetCurrentWorld().GetEntity((entt::entity)id);
	}

	World& SceneCommand::GetCurrentWorld()
	{
		return Application::GetModule<SceneHandler>().GetCurrentWorld();
	}

	void SceneCommand::RenderScene(Rendering::Camera camera) {

		RenderCommand::Begin(camera);
		GetCurrentWorld().RenderAll();
		RenderCommand::End();
	}

	//Submit element to RenderEngine
	template<typename C, typename T>
	void SceneCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}
	template<>
	void SceneCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		RenderCommand::Submit(Quad(transform.GetTransformMat4(), component.m_Tint, component.m_Texture));
	}
	template<>
	void SceneCommand::Render(MeshComponent& component, TransformComponent& transform) {
		RenderCommand::Submit(RenderableMesh(component.m_Mesh, transform.GetTransformMat4()));
	}

	template<>
	void SceneCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (int x = 0; x < component.m_Size; x++) {
			for (int y = 0; y < component.m_Size; y++) {
				Quad quad = { transform.GetTransformMat4() * glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f)),
					component.m_Tint, nullptr };
				RenderCommand::Submit(quad);
			}
		}
	}
}
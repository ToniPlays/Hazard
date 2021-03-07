#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "../Component.h"
#include "SceneCommand.h"

namespace Hazard::ECS {

	Rendering::RenderEngine SceneCommand::RenderEngine;

	void SceneCommand::Init()
	{
		RenderEngine = Application::GetModule<Rendering::RenderEngine>();
	}

	void SceneCommand::OnScriptAttached(Entity& entity, ScriptComponent& script)
	{
		Scripting::ScriptCommand::InitScripEntity(entity, script);
	}

	void SceneCommand::OnScriptDetached(Entity& entity, ScriptComponent& script)
	{
		Scripting::ScriptCommand::RemoveScriptableEntity(entity, script);
	}

	template<typename C, typename T>
	void SceneCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}

	template<>
	void SceneCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		RenderEngine.Submit(Rendering::Quad(transform.GetTransformMat4(), component.tint.ToGlm(), component.texture));
	}

	template<>
	void SceneCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (int x = 0; x < component.size; x++) {
			for (int y = 0; y < component.size; y++) {
				RenderEngine.Submit(Rendering::Quad { transform.GetTransformMat4() * glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f)),
					component.tint.ToGlm(), nullptr });
			}
		}
	}
}
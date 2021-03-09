#pragma once

#include <hzrpch.h>
#include "SceneCommand.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Core/Application.h"
#include "../Component.h"
#include "SceneHandler.h"

namespace Hazard::ECS {

	Rendering::RenderEngine SceneCommand::RenderEngine;
	//Initialize connection to renderEngine
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

	Scene& SceneCommand::GetCurrentScene()
	{
		return Application::GetModule<SceneHandler>().GetCurrentScene();
	}

	//Submit element to RenderEngine
	template<typename C, typename T>
	void SceneCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}
	template<>
	void SceneCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		Rendering::RenderCommand::Submit(Rendering::Quad(transform.GetTransformMat4(), component.tint.ToGlm(), component.texture));
	}

	template<>
	void SceneCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (int x = 0; x < component.size; x++) {
			for (int y = 0; y < component.size; y++) {
				Rendering::RenderCommand::Submit(Rendering::Quad { transform.GetTransformMat4() * glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f)),
					component.tint.ToGlm(), nullptr });
			}
		}
	}
}
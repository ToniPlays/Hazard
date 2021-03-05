#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "../Component.h"
#include "SceneCommand.h"

namespace Hazard::ECS {

	Rendering::RenderEngine SceneCommand::engine;

	void SceneCommand::Init()
	{
		engine = Application::GetModule<Rendering::RenderEngine>();
	}

	template<typename C, typename T>
	void SceneCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}

	template<>
	void SceneCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		engine.Submit(Rendering::Quad(transform.GetTransformMat4(), component.tint.ToGlm(), component.texture));
	}

	template<>
	void SceneCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (int x = 0; x < component.size; x++) {
			for (int y = 0; y < component.size; y++) {
				glm::vec3 translation = transform.Translation + glm::vec3{ x * 1.1, y * 1.1, 1.0 };
				engine.Submit(Rendering::Quad{ Math::ToTransformMatrix(translation, transform.Rotation, transform.Scale),
					component.tint.ToGlm(), nullptr });
			}
		}
	}
}
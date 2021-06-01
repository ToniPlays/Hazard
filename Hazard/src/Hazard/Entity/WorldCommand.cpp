#pragma once

#include <hzrpch.h>
#include "WorldCommand.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Core/Application.h"
#include "Component.h"
#include "WorldHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"

namespace Hazard::ECS {

	using namespace Hazard::Rendering;

	template<typename T>
	void WorldCommand::OnScriptAttached(Entity& entity, T& script)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, ScriptComponent& script)  {
		Scripting::ScriptCommand::InitEntity(entity, script);
	}
	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, VisualScriptComponent& script)  {}
	template<typename T>
	void WorldCommand::OnScriptDetached(Entity& entity, T& script)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::OnScriptDetached(Entity& entity, ScriptComponent& script)
	{
		Scripting::ScriptCommand::ClearEntity(entity, script);
	}
	template<>
	void WorldCommand::OnScriptDetached(Entity& entity, VisualScriptComponent& script)
	{
		Scripting::ScriptCommand::ClearEntity(entity, script);
	}

	Entity WorldCommand::GetEntity(uint32_t id)
	{
		return GetCurrentWorld().GetEntity((entt::entity)id);
	}

	World& WorldCommand::GetCurrentWorld()
	{
		return Application::GetModule<WorldHandler>()->GetCurrentWorld();
	}

	void WorldCommand::RenderScene(Rendering::Camera camera) {

		RenderCommand::Begin(camera, GetCurrentWorld());
		GetCurrentWorld().RenderAll();
		RenderCommand::End();
	}

	//Submit element to RenderEngine
	template<typename C, typename T>
	void WorldCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		RenderCommand::Submit(Quad(transform.GetTransformMat4(), component.m_Tint, component.m_Texture.Raw()));
	}
	template<>
	void WorldCommand::Render(MeshComponent& component, TransformComponent& transform) {
		RenderCommand::Submit(RenderableMesh(component.m_Mesh.Raw(), component.m_Material.Raw(), transform.GetTransformMat4()));
	}

	template<>
	void WorldCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (uint32_t x = 0; x < component.m_Size; x++) {
			for (uint32_t y = 0; y < component.m_Size; y++) {
				Quad quad = { transform.GetTransformMat4() * glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f)),
					component.m_Tint, nullptr };
				RenderCommand::Submit(quad);
			}
		}
	}
}
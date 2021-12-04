#pragma once

#include <hzrpch.h>
#include "WorldCommand.h"
#include "Hazard/Core/Application.h"
#include "Component.h"
#include "WorldHandler.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Physics/PhysicsCommand.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/2D/Renderer2D.h"

namespace Hazard::ECS {

	using namespace Hazard::Rendering;

	template<typename T>
	void WorldCommand::OnScriptAttached(Entity& entity, T& script)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, ScriptComponent& script) {
		Scripting::ScriptCommand::InitEntity(entity, script);
	}
	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, VisualScriptComponent& script) {}
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

	void WorldCommand::Init()
	{
		m_Handler = &Application::GetModule<WorldHandler>();
		RenderCommand::AddRenderCallback(ProcessWorld);
	}

	Entity WorldCommand::GetEntity(uint32_t id)
	{
		return GetCurrentWorld()->GetEntity((entt::entity)id);
	}

	Ref<World> WorldCommand::GetCurrentWorld()
	{
		return m_Handler->GetCurrentWorld();
	}

	void WorldCommand::ProcessWorld()
	{
		HZ_SCOPE_PERF("WorldCommand::ProcessWorld");
		Ref<World> world = GetCurrentWorld();
		auto spriteRenderers = world->GetWorldRegistry().group<SpriteRendererComponent>(entt::get<TransformComponent>);

		for (auto entity : spriteRenderers) {

			Entity e = { entity, world.Raw() };
			if (!e.IsVisible()) continue;

			auto& [sprite, transform] = spriteRenderers.get<SpriteRendererComponent, TransformComponent>(entity);
			RenderCommand::DrawQuadTextured(sprite, transform);
		}


		auto batches = world->GetWorldRegistry().group<BatchComponent>(entt::get<TransformComponent>);
		for (auto entity : batches)
		{
			Entity e = { entity, world.Raw() };
			if (!e.IsVisible()) continue;
			auto& [batch, tc] = batches.get<BatchComponent, TransformComponent>(entity);

			for (size_t x = 0; x < batch.m_Size; x++) {
				for (size_t y = 0; y < batch.m_Size; y++) {
					RenderCommand::DrawQuad(Math::ToTransformMatrix({ x, y, 0.0f }), batch.m_Tint);
				}
			}
		}
	}

	//Submit element to RenderEngine
	template<typename C, typename T>
	void WorldCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::Render(SpriteRendererComponent& component, TransformComponent& transform) {
		RenderCommand::DrawQuad(component, transform);
	}
	template<>
	void WorldCommand::Render(MeshComponent& component, TransformComponent& transform) {
		//RenderCommand::Submit(RenderableMesh(component.m_Mesh.Raw(), component.m_Material.Raw(), transform.GetTransformMat4()));
	}

	template<>
	void WorldCommand::Render(BatchComponent& component, TransformComponent& transform) {
		for (uint32_t x = 0; x < component.m_Size; x++) {
			for (uint32_t y = 0; y < component.m_Size; y++) {
				//Quad quad = { transform.GetTransformMat4() * glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f)),
				//	component.m_Tint, nullptr };
				//RenderCommand::Submit(quad);
			}
		}
	}
}
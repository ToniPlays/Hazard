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
		Scripting::ScriptCommand::InitEntity((uint32_t)entity, script);
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
		Scripting::ScriptCommand::ClearEntity((uint32_t)entity, script);
	}
	template<>
	void WorldCommand::OnScriptDetached(Entity& entity, VisualScriptComponent& script)
	{
		Scripting::ScriptCommand::ClearEntity((uint32_t)entity, script);
	}

	void WorldCommand::Init()
	{
		m_Handler = &Application::GetModule<WorldHandler>();
	}

	Entity WorldCommand::GetEntity(uint32_t id)
	{
		return GetCurrentWorld()->GetEntity((entt::entity)id);
	}
	void WorldCommand::WorldRuntimeBegin()
	{
		using namespace Physics;

		World* world = GetCurrentWorld().Raw();

		auto& rb2dView = world->GetWorldRegistry().view<Rigidbody2DComponent>();

		for (auto& e : rb2dView)
		{
			Entity entity = { e, world };
			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			Physics2DObjectCreateInfo objInfo = {};
			objInfo.Handle = (uint32_t)entity;
			objInfo.Position = { tc.m_Translation.x, tc.m_Translation.y };
			objInfo.Angle = tc.m_Rotation.z;
			objInfo.BodyType = rb2d.Type;
			objInfo.FixedRotation = rb2d.FixedRotation;
			objInfo.GravityScale = rb2d.UseGravity ? 1.0f : 0.0f;

			rb2d.runtimeBody = PhysicsCommand::CreateObject(&objInfo);

			if (entity.HasComponent<BoxCollider2DComponent>()) {
				BoxCollider2DComponent& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				PhysicsCollider2DCreateInfo info = {};

				info.Handle = (uint32_t)entity;
				info.Body = rb2d.runtimeBody;
				info.Type = ColliderType::Box;
				info.Scale = { tc.m_Scale.x, tc.m_Scale.y };
				info.Size = bc2d.Size;
				info.Density = bc2d.Density;
				info.Friction = bc2d.Friction;
				info.Restitution = bc2d.Restitution;
				info.RestitutionThreshold = bc2d.RestitutionThreshold;
				info.IsSensor = bc2d.IsSensor;

				bc2d.runtimeFixture = PhysicsCommand::CreateCollider(&info);
			}
		}
	}
	void WorldCommand::WorldRuntimeEnd()
	{

	}

	Ref<World> WorldCommand::GetCurrentWorld()
	{
		return m_Handler->GetCurrentWorld();
	}

	void WorldCommand::RenderWorld()
	{
		Ref<World> world = GetCurrentWorld();

		HZ_SCOPE_PERF("WorldCommand::ProcessWorld");

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

	void WorldCommand::UpdatePhysics()
	{
		Ref<World> world = GetCurrentWorld();
		auto& rbView = world->FindEntitiesWith<Rigidbody2DComponent>();

		for (auto& [entityID, rb2d] : rbView)
		{
			Entity entity = { entityID, world.Raw() };
			auto& tc = entity.GetComponent<TransformComponent>();

			glm::vec2 pos = Physics::PhysicsCommand::GetPosition(rb2d.runtimeBody);
			float angle = Physics::PhysicsCommand::GetAngle(rb2d.runtimeBody);

			tc.m_Translation.x = pos.x;
			tc.m_Translation.y = pos.y;
			tc.m_Rotation.z = angle;
		}
	}

	//Submit element to RenderEngine
	template<typename C, typename T>
	void WorldCommand::Render(C& component, T& transform)
	{
		static_assert(false);
	}
	template<>
	void WorldCommand::Render(SpriteRendererComponent& component, TransformComponent& transform)
	{
		RenderCommand::DrawQuad(component, transform);
	}
	template<>
	void WorldCommand::Render(MeshComponent& component, TransformComponent& transform)
	{

	}

	template<>
	void WorldCommand::Render(BatchComponent& component, TransformComponent& transform)
	{

	}
}
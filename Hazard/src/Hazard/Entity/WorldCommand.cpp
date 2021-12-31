#pragma once

#include <hzrpch.h>
#include "WorldCommand.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "Hazard/Core/Application.h"
#include "Component.h"
#include "WorldHandler.h"
#include "Hazard/Audio/AudioCommand.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Physics/PhysicsCommand.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/2D/Renderer2D.h"

#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::ECS {

	using namespace Hazard::Rendering;

	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, ScriptComponent& script) {
		Scripting::ScriptCommand::InitEntity((uint32_t)entity, script);
	}
	template<>
	void WorldCommand::OnScriptAttached(Entity& entity, VisualScriptComponent& script) {}
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

		Ref<World> world = GetCurrentWorld();

		const auto& rb2dView = world->GetEntitiesWith<Rigidbody2DComponent>();
		for (auto& e : rb2dView)
		{
			Entity entity = { e, world.Raw() };
			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			Physics2DObjectCreateInfo objInfo = {};
			objInfo.Handle = (uint32_t)entity;
			objInfo.Position = { tc.Translation.x, tc.Translation.y };
			objInfo.Angle = tc.Rotation.z;
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
				info.Scale = { tc.Scale.x, tc.Scale.y };
				info.Offset = bc2d.Offset;
				info.Size = bc2d.Size;
				info.Density = bc2d.Density;
				info.Friction = bc2d.Friction;
				info.Restitution = bc2d.Restitution;
				info.RestitutionThreshold = bc2d.RestitutionThreshold;
				info.IsSensor = bc2d.IsSensor;

				bc2d.runtimeFixture = PhysicsCommand::CreateCollider(&info);
			}
			if (entity.HasComponent<CircleCollider2DComponent>()) {
				CircleCollider2DComponent& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				PhysicsCollider2DCreateInfo info = {};

				info.Handle = (uint32_t)entity;
				info.Body = rb2d.runtimeBody;
				info.Type = ColliderType::Circle;
				info.Scale = { tc.Scale.x, tc.Scale.y };
				info.Offset = cc2d.Offset;
				info.Radius = cc2d.Radius;
				info.Density = cc2d.Density;
				info.Friction = cc2d.Friction;
				info.Restitution = cc2d.Restitution;
				info.RestitutionThreshold = cc2d.RestitutionThreshold;
				info.IsSensor = cc2d.IsSensor;

				cc2d.runtimeFixture = PhysicsCommand::CreateCollider(&info);
			}
		}
		for (auto& e : world->GetEntitiesWith<AudioSourceComponent>())
		{
			Entity entity = { e, world.Raw() };
			auto& as = entity.GetComponent<AudioSourceComponent>();

			if (as.PlayOnCreate)
				as.Source.Play();

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

			const auto& [sprite, transform] = spriteRenderers.get<SpriteRendererComponent, TransformComponent>(entity);
			RenderCommand::DrawQuad(transform.GetTransformMat4(), sprite.Tint, sprite.Texture);
		}

		auto batches = world->GetWorldRegistry().group<BatchComponent>(entt::get<TransformComponent>);

		for (auto entity : batches)
		{
			Entity e = { entity, world.Raw() };
			if (!e.IsVisible()) continue;
			const auto& [batch, tc] = batches.get<BatchComponent, TransformComponent>(entity);

			for (size_t x = 0; x < batch.Size; x++) {
				for (size_t y = 0; y < batch.Size; y++) {
					RenderCommand::DrawQuad(Math::ToTransformMatrix({ x, y, 0.0f }), batch.Tint);
				}
			}
		}
		auto meshes = world->GetWorldRegistry().group<MeshComponent>(entt::get<TransformComponent>);

		for (auto entity : meshes)
		{
			Entity e = { entity, world.Raw() };
			if (!e.IsVisible()) continue;

			auto& [mesh, tc] = meshes.get<MeshComponent, TransformComponent>(entity);
			if (mesh.m_Mesh)
				RenderCommand::DrawMesh(mesh.m_Mesh, tc.GetTransformMat4());
		}
	}

	void WorldCommand::UpdatePhysics()
	{
		Ref<World> world = GetCurrentWorld();
		const auto& rbView = world->GetEntitiesWith<Rigidbody2DComponent>();

		for (auto& e : rbView)
		{
			Entity entity = { e, world.Raw() };
			auto& tc = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			glm::vec2 pos = Physics::PhysicsCommand::GetPosition(rb2d.runtimeBody);
			float angle = Physics::PhysicsCommand::GetAngle(rb2d.runtimeBody);

			tc.Translation.x = pos.x;
			tc.Translation.y = pos.y;
			tc.Rotation.z = angle;
		}
	}
}
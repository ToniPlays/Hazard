#pragma once
#include <hzrpch.h>
#include "PhysicsCommand.h"
#include "Hazard/Entity/Entity.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world_callbacks.h"

namespace Hazard::Physics 
{
	Physics2D* PhysicsCommand::s_Physics2D = nullptr;

	void PhysicsCommand::Init()
	{
		s_Physics2D = new Physics2D();
	}
	void PhysicsCommand::OnBeginRuntime(float gravity, ECS::World* world)
	{
		s_Physics2D->Begin(gravity);
		s_Physics2D->SetIterations(6, 2);
		auto view = world->GetWorldRegistry().view<ECS::Rigidbody2DComponent>();

		for (auto entityID : view) 
		{
			ECS::Entity entity = { entityID, world };
			auto& tc = entity.GetComponent<ECS::TransformComponent>();
			auto& rb = entity.GetComponent<ECS::Rigidbody2DComponent>();

			rb.runtimeBody = s_Physics2D->CreateBody((uint32_t)entityID, (b2BodyType)rb.type, tc.m_Translation, tc.m_Rotation, rb.UseGravity, rb.FixedRotation);

			if (entity.HasComponent<ECS::BoxCollider2DComponent>()) 
			{
				auto& bc = entity.GetComponent<ECS::BoxCollider2DComponent>();
				s_Physics2D->CreateBoxCollider(rb.runtimeBody, { bc.Size.x * tc.m_Scale.x, bc.Size.y * tc.m_Scale.y }, bc.Density,
					bc.Friction, bc.Restitution, bc.RestitutionThreshold, bc.IsSensor);
			}
		}
	}
	void PhysicsCommand::OnEndRuntime()
	{
		s_Physics2D->End();
	}
	void PhysicsCommand::UpdateAll(Ref<ECS::World> world)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		auto view = world->GetWorldRegistry().view<ECS::Rigidbody2DComponent>();

		for (auto entityID : view)
		{
			ECS::Entity entity{ entityID, world.Raw()};
			auto& tc = entity.GetComponent<ECS::TransformComponent>();
			auto& rb = entity.GetComponent<ECS::Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb.runtimeBody;
			body->SetTransform({ tc.m_Translation.x, tc.m_Translation.y }, tc.m_Rotation.z);
		}

		s_Physics2D->Step(Time::s_DeltaTime);
		for (auto entityID : view)
		{
			ECS::Entity entity { entityID, world.Raw() };
			auto& tc = entity.GetComponent<ECS::TransformComponent>();
			auto& rb = entity.GetComponent<ECS::Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb.runtimeBody;
			const auto& position = body->GetPosition();
			tc.m_Translation.x = position.x;
			tc.m_Translation.y = position.y;
			tc.m_Rotation.z = body->GetAngle();
		}
	}
}
#pragma once
#include <hzrpch.h>
#include "PhysicsCommand.h"
#include "Hazard/Entity/Entity.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Hazard::Physics 
{
	b2World* PhysicsCommand::s_World2D = nullptr;
	void PhysicsCommand::OnBeginRuntime(float gravity, ECS::World* world)
	{
		s_World2D = new b2World({ 0.0f, gravity });

		auto view = world->GetWorldRegistry().view<ECS::Rigidbody2DComponent>();

		for (auto entityID : view) 
		{
			ECS::Entity entity = { entityID, world };
			auto& tc = entity.GetComponent<ECS::TransformComponent>();
			auto& rb = entity.GetComponent<ECS::Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rb.type;
			bodyDef.position.Set(tc.m_Translation.x, tc.m_Translation.y);
			bodyDef.angle = tc.m_Rotation.z;

			b2Body* body = s_World2D->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.FixedRotation);
			rb.runtimeBody = body;

			if (entity.HasComponent<ECS::BoxCollider2DComponent>()) 
			{
				auto& bc = entity.GetComponent<ECS::BoxCollider2DComponent>();
				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc.Size.x * tc.m_Scale.x, bc.Size.y * tc.m_Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc.Density;
				fixtureDef.friction = bc.Friction;
				fixtureDef.restitution = bc.Restitution;
				fixtureDef.restitutionThreshold = bc.RestitutionTreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}
	void PhysicsCommand::OnEndRuntime()
	{
		delete s_World2D;
		s_World2D = nullptr;
	}
	void PhysicsCommand::UpdateAll(ECS::World* world)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		s_World2D->Step(Time::s_DeltaTime, velocityIterations, positionIterations);

		auto view = world->GetWorldRegistry().view<ECS::Rigidbody2DComponent>();

		for (auto entityID : view)
		{
			ECS::Entity entity { entityID, world };
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
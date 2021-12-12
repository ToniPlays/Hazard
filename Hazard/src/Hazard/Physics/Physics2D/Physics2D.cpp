#pragma once
#include "hzrpch.h"
#include "Physics2D.h"
#include "Physics2DContactListener.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world_callbacks.h"

namespace Hazard::Physics 
{
	void Physics2D::Begin(float gravity)
	{
		m_World2D = new b2World({ 0.0f, gravity });
		Physics2DContactListener* listener = new Physics2DContactListener(this);
		m_World2D->SetContactListener(listener);

	}
	void Physics2D::Step(float timeStep)
	{
		m_World2D->Step(timeStep, velocityIterations, positionIterations);
	}
	void Physics2D::End()
	{
		delete m_World2D;
		m_World2D = nullptr;
	}
	void* Physics2D::CreatePhysicsObject(Physics2DObjectCreateInfo* createInfo)
	{
		uint32_t* data = (uint32_t*)&createInfo->Handle;
		b2BodyUserData userData;
		userData.pointer = *data;

		b2BodyDef bodyDef;
		bodyDef.type = (b2BodyType)createInfo->BodyType;
		bodyDef.position.Set(createInfo->Position.x, createInfo->Position.y);
		bodyDef.angle = createInfo->Angle;
		bodyDef.gravityScale = createInfo->GravityScale;
		bodyDef.enabled = createInfo->Enabled;
		bodyDef.userData = userData;

		b2Body* body = m_World2D->CreateBody(&bodyDef);
		body->SetFixedRotation(createInfo->FixedRotation);
		return body;
	}
	void* Physics2D::CreateCollider(PhysicsCollider2DCreateInfo* createInfo)
	{
		if (createInfo->Body == nullptr) 
		{
			HZR_CORE_WARN("Cannot create Collider for null body");
			return nullptr;
		}

		b2FixtureDef fixture;

		switch (createInfo->Type) {
		case ColliderType::Box: 
		{
			b2PolygonShape box;
			box.SetAsBox(createInfo->Size.x * createInfo->Scale.x, createInfo->Size.y * createInfo->Scale.y, 
				b2Vec2(createInfo->Offset.x, createInfo->Offset.y), 0.0f);

			fixture.shape = &box;
			fixture.density = createInfo->Density;
			fixture.friction = createInfo->Friction;
			fixture.restitution = createInfo->Restitution;
			fixture.restitutionThreshold = createInfo->RestitutionThreshold;
			fixture.isSensor = createInfo->IsSensor;

			return ((b2Body*)createInfo->Body)->CreateFixture(&fixture);
		}
		case ColliderType::Circle: 
		{
			b2CircleShape circle;
			circle.m_p.Set(createInfo->Offset.x, createInfo->Offset.y);

			fixture.shape = &circle;
			fixture.density = createInfo->Density;
			fixture.friction = createInfo->Friction;
			fixture.restitution = createInfo->Restitution;
			fixture.restitutionThreshold = createInfo->RestitutionThreshold;
			fixture.isSensor = createInfo->IsSensor;
			
			return ((b2Body*)createInfo->Body)->CreateFixture(&fixture);
		}
		case ColliderType::Capsule: 
		{
		
		}
		case ColliderType::Custom: 
		{

		}
		}
		return nullptr;
	}
}

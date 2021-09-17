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
		Physics2DContactListener* listener = new Physics2DContactListener();
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
	void* Physics2D::CreateBody(uint32_t entityID, b2BodyType type, glm::vec3 position, glm::vec3 rotation, bool useGravity, bool fixedRotation)
	{
		uint32_t* data = (uint32_t*)&entityID;
		HZR_CORE_INFO(*data);
		b2BodyUserData userData;
		userData.pointer = *data;

		b2BodyDef bodyDef;
		bodyDef.type = type;
		bodyDef.position.Set(position.x, position.y);
		bodyDef.angle = rotation.z;
		bodyDef.userData = userData;
		bodyDef.gravityScale = useGravity;

		b2Body* body = m_World2D->CreateBody(&bodyDef);
		body->SetFixedRotation(fixedRotation);
		return body;
	}
	void Physics2D::CreateBoxCollider(void* body, glm::vec2 size, float density, float friction, float restitution, float restitutionThreshold, bool sensor)
	{
		b2PolygonShape boxShape;
		boxShape.SetAsBox(size.x, size.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = density;
		fixtureDef.friction = friction;
		fixtureDef.restitution = restitution;
		fixtureDef.restitutionThreshold = restitutionThreshold;
		fixtureDef.isSensor = sensor;

		((b2Body*)body)->CreateFixture(&fixtureDef);
	}
}

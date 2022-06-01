
#include <hzrpch.h>
#include "Physics2DContactListener.h"
#include "box2d/b2_contact.h"

#include "Hazard/Entity/WorldCommand.h"

namespace Hazard::Physics
{
	void Physics2DContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		b2Body* bodyA = a->GetBody();
		b2Body* bodyB = b->GetBody();

		uint32_t entityAID = (uint32_t)bodyA->GetUserData().pointer;
		uint32_t entityBID = (uint32_t)bodyB->GetUserData().pointer;
		

		//Collider
		Contact2DData dataA;
		dataA.State = CollisionState::ContactBegin;
		dataA.Collider = entityAID;
		dataA.CollidedTo = entityBID;
		dataA.Type = a->IsSensor() ? ContactType::Sensor : ContactType::Collision;

		//Collider
		Contact2DData dataB;
		dataB.State = CollisionState::ContactBegin;
		dataB.Collider = entityBID;
		dataB.CollidedTo = entityAID;
		dataB.Type = b->IsSensor() ? ContactType::Sensor : ContactType::Collision;

		m_Physics->OnContact(dataA);
		m_Physics->OnContact(dataB);
	}
	void Physics2DContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		b2Body* bodyA = a->GetBody();
		b2Body* bodyB = b->GetBody();

		uint32_t entityAID = (uint32_t)bodyA->GetUserData().pointer;
		uint32_t entityBID = (uint32_t)bodyB->GetUserData().pointer;

		//Collider
		Contact2DData dataA;
		dataA.State = CollisionState::ContactEnd;
		dataA.Collider = entityAID;
		dataA.CollidedTo = entityBID;
		dataA.Type = a->IsSensor() ? ContactType::Sensor : ContactType::Collision;

		//Collider
		Contact2DData dataB;
		dataB.State = CollisionState::ContactEnd;
		dataB.Collider = entityBID;
		dataB.CollidedTo = entityAID;
		dataB.Type = b->IsSensor() ? ContactType::Sensor : ContactType::Collision;

		m_Physics->OnContact(dataA);
		m_Physics->OnContact(dataB);
	}
}

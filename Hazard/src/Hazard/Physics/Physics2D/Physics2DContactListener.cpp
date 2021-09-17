#pragma once
#include <hzrpch.h>
#include "Physics2DContactListener.h"
#include "box2d/b2_contact.h"

#include "Hazard/Entity/WorldCommand.h"
#include "Scripting/ScriptEngines.h"
#include "Hazard/Scripting/ScriptCommand.h"

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

		ECS::Entity entityA = ECS::WorldCommand::GetEntity(entityAID);
		ECS::Entity entityB = ECS::WorldCommand::GetEntity(entityBID);

		Collider2DData AData;
		AData.entityID = entityA;
		Collider2DData BData;
		BData.entityID = entityB;

		void* paramA[] = { &AData };
		void* paramB[] = { &BData };

		if (a->IsSensor() || b->IsSensor()) {
			if (a->IsSensor() && !b->IsSensor())
				ProcessEntityEnter(CollisionType::Sensor, entityAID, paramB);
			if (b->IsSensor() && !a->IsSensor())
				ProcessEntityEnter(CollisionType::Sensor, entityBID, paramA);
			return;
		}
		ProcessEntityEnter(CollisionType::Collision, entityAID, paramB);
		ProcessEntityEnter(CollisionType::Collision, entityBID, paramA);
	}
	void Physics2DContactListener::EndContact(b2Contact* contact)
	{
		using namespace Scripting;
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		b2Body* bodyA = a->GetBody();
		b2Body* bodyB = b->GetBody();

		uint32_t entityAID = (uint32_t)bodyA->GetUserData().pointer;
		uint32_t entityBID = (uint32_t)bodyB->GetUserData().pointer;

		ECS::Entity entityA = ECS::WorldCommand::GetEntity(entityAID);
		ECS::Entity entityB = ECS::WorldCommand::GetEntity(entityBID);
	}
	void Physics2DContactListener::ProcessEntityEnter(CollisionType type, uint32_t entityID, void** param)
	{
		using namespace Scripting;
		ECS::Entity entity = ECS::WorldCommand::GetEntity(entityID);
		CSharp::CSharpEngine& engine = (CSharp::CSharpEngine&)ScriptCommand::GetEngine(ScriptType::CSharpScript);
		if (!engine.EntityInstanceExits(entityID)) return;

		CSharp::EntityInstanceData& instance = engine.GetInstanceData(entityID);
		if (instance.instance.ScriptClass == nullptr) return;

		switch (type)
		{
		case Hazard::Physics::CollisionType::Collision:
			CSharp::Mono::TryCallMethod(instance.instance.GetInstance(), instance.instance.ScriptClass->OnColliderEnter2D, param);
			break;
		case Hazard::Physics::CollisionType::Sensor:
			CSharp::Mono::TryCallMethod(instance.instance.GetInstance(), instance.instance.ScriptClass->OnTriggerEnter2D, param);
			break;
		}
	}
}

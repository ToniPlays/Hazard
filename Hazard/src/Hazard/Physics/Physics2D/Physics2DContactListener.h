#pragma once
#include "box2d/b2_world.h"
#include "box2d/b2_world_callbacks.h"

namespace Hazard::Physics 
{
	struct Collider2DData 
	{
		uint32_t entityID;
	};

	enum class CollisionType { Collision = 0, Sensor };
	class Physics2DContactListener : public b2ContactListener {
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
	private:
		void ProcessEntityEnter(CollisionType type, uint32_t entityID, void** param);
	};
}
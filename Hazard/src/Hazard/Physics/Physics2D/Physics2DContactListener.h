#pragma once
#include "box2d/b2_world.h"
#include "box2d/b2_world_callbacks.h"



namespace Hazard::Physics 
{
	class Physics2D;

	class Physics2DContactListener : public b2ContactListener {
	public:

		Physics2DContactListener(Physics2D* physics2D) : m_Physics(physics2D) {};
		~Physics2DContactListener() = default;

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		Physics2D* m_Physics;
	};
}
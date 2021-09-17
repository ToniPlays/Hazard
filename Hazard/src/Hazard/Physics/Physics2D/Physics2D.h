#pragma once

#include "box2d/box2d.h"

namespace Hazard::Physics {
	class Physics2D {
	public:
		Physics2D() = default;
		~Physics2D() = default;

		void SetIterations(int32_t velocity, int32_t position) {
			velocityIterations = velocity;
			positionIterations = position;
		}

		void Begin(float gravity);
		void Step(float timeStep);
		void End();

		void* CreateBody(uint32_t entityID, b2BodyType type, glm::vec3 position, glm::vec3 rotation, bool useGravity, bool fixedRotation);
		void CreateBoxCollider(void* body, glm::vec2 size, float density, float friction, float restitution, float restitutionThreshold, bool sensor);
		b2World* GetWorld() { return m_World2D; }

	private:
		b2World* m_World2D = nullptr;
		int32_t velocityIterations = 6;
		int32_t positionIterations = 2;
	};
}
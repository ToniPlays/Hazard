#pragma once

#include "box2d/box2d.h"

namespace Hazard::Physics
{
	enum class ContactType { Collision = 0, Sensor };
	enum class CollisionState { ContactBegin = 0, ContactEnd };
	enum class BodyType { Static = 0, Kinematic, Dynamic };
	enum class ColliderType { Box, Circle, Capsule, Custom };

	struct Contact2DData
	{
		uint32_t Collider;
		uint32_t CollidedTo;
		ContactType Type;
		CollisionState State;
	};

	struct Physics2DObjectCreateInfo
	{
		uint32_t Handle;
		BodyType BodyType;
		glm::vec2 Position;
		float Angle = 0.0f;
		float GravityScale = 1.0f;
		bool Enabled = true;
		bool FixedRotation = false;
	};

	struct PhysicsCollider2DCreateInfo
	{
		uint32_t Handle;
		ColliderType Type;
		glm::vec2 Scale = { 1.0f, 1.0f };;
		glm::vec2 Size = { 1.0f, 1.0f };
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius;
		float Density;
		float Friction;
		float Restitution;
		float RestitutionThreshold;
		bool IsSensor = false;

		void* Body = nullptr;
	};


	using ContactCallback = std::function<void(Contact2DData)>;

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


		void* CreatePhysicsObject(Physics2DObjectCreateInfo* createInfo);
		void* CreateCollider(PhysicsCollider2DCreateInfo* createInfo);
		void AddContactCallback(ContactCallback callback) { m_Callbacks.push_back(callback); };

		void OnContact(Contact2DData data)
		{
			for (auto& callback : m_Callbacks) {
				callback(data);
			}
		}

		glm::vec2 GetPosition(void* handle) {
			b2Vec2 pos = ((b2Body*)handle)->GetPosition();
			return { pos.x, pos.y };
		};
		float GetAngle(void* handle) { return ((b2Body*)handle)->GetAngle(); }

		b2World* GetWorld() { return m_World2D; }

	private:
		b2World* m_World2D = nullptr;
		int32_t velocityIterations = 6;
		int32_t positionIterations = 2;

		std::vector<ContactCallback> m_Callbacks;
	};
}
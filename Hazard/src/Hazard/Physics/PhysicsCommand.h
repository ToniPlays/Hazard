#pragma once

#include <box2d/box2d.h>
#include "Hazard/ECS/World.h"
#include "Physics2D/Physics2D.h"

namespace Hazard::Physics
{
	std::string BodyTypeToString(BodyType type);
	BodyType StringToBodyType(const std::string& value);

	struct PhysicsBeginInfo 
	{
		float Gravity = -9.81f;
	};


	class PhysicsCommand {
	public:
		static void Init();

		static void* CreateObject(Physics2DObjectCreateInfo* createInfo) { return s_Physics2D->CreatePhysicsObject(createInfo); };
		static void* CreateCollider(PhysicsCollider2DCreateInfo* createInfo) { return s_Physics2D->CreateCollider(createInfo); };
		static void BeginSimulation(PhysicsBeginInfo* info);
		static void StopSimulation();

		static void AddContact2DCallback(ContactCallback callback) { s_Physics2D->AddContactCallback(callback); }

		static glm::vec2 GetPosition(void* handle) { return s_Physics2D->GetPosition(handle); };
		static float GetAngle(void* handle) { return s_Physics2D->GetAngle(handle); };

		static void SetGravity(float gravity) {};
		static void Step(double delta);
	private:
		static Physics2D* s_Physics2D;
	};
}
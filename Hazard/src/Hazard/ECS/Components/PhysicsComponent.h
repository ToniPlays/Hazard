#pragma once

#include "glm/glm.hpp"
#include "Hazard/Physics/Physics2D/Physics2D.h"
#include "ComponentBase.h"

namespace Hazard
{
	struct Rigidbody2DComponent : ComponentBase
	{
		Physics::BodyType Type = Physics::BodyType::Static;

		bool FixedRotation = false;
		bool UseGravity = true;

		void* runtimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};
	struct BoxCollider2DComponent : ComponentBase
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionThreshold = 0.5f;

		bool IsSensor = false;
		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
	struct CircleCollider2DComponent : ComponentBase
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionThreshold = 0.5f;

		bool IsSensor = false;
		void* runtimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};
	struct RigidbodyComponent : ComponentBase 
	{

	};
	struct BoxColliderComponent : ComponentBase 
	{

	};
	struct SphereColliderComponent : ComponentBase
	{

	};
}
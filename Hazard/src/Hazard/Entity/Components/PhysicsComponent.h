#pragma once

#include "glm/glm.hpp"

namespace Hazard::ECS {
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };
		BodyType type = BodyType::Static;

		bool FixedRotation = false;
		void* runtimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};
	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.5f;
		float RestitutionTreshold = 0.5f;

		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}
#pragma once

#include "glm/glm.hpp"

namespace Hazard::ECS
{
	enum class BodyType { Static = 0, Kinematic, Dynamic };

	std::string BodyTypeToString(BodyType type);
	BodyType StringToBodyType(const std::string& value);

	struct Rigidbody2DComponent
	{
		BodyType type = BodyType::Static;

		bool FixedRotation = false;
		bool UseGravity = true;
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
		float RestitutionThreshold = 0.5f;

		bool IsSensor = false;
		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}
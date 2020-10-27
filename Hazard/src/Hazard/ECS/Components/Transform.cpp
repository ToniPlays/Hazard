#pragma once

#include <hzrpch.h>
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"


namespace Hazard {

	Transform::Transform() : Component("Transform") {
		this->position = Vector3<float>::zero;
		this->rotation = Vector3<float>::zero;
		this->scale = Vector3<float>::one;
	}

	Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale) : Component("Transform")
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	glm::mat4 Transform::AsMat4(Transform _transform)
	{
		glm::vec3 rotation = Vector3<float>::AsGLM(_transform.rotation);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Vector3<float>::AsGLM(_transform.position));

		glm::mat4 rot =  glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(0, 0, 1))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(1, 0, 0));

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), Vector3<float>::AsGLM(_transform.scale));
		return transform * rot * scale;
	}
	void Transform::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "Transform" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value; Serializer::Serialize(out, position);
		out << YAML::Key << "Rotation" << YAML::Value; Serializer::Serialize(out, rotation);
		out << YAML::Key << "Scale"    << YAML::Value; Serializer::Serialize(out, scale);
		out << YAML::EndMap;
	}
	void Transform::DeserializeComponent(YAML::Node in) {

		this->position = Deserializer::Deserialize<Vector3<float>>(in["Position"]);
		this->rotation = Deserializer::Deserialize<Vector3<float>>(in["Rotation"]);
		this->scale    = Deserializer::Deserialize<Vector3<float>>(in["Scale"]);
	}
}
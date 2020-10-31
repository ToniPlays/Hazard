#pragma once
#include <hzrpch.h>
#include "Transform.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"

#include <glm/gtx/quaternion.hpp>

namespace Hazard {

	Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale) : Component("Transform") {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	glm::mat4 Transform::AsMat4(Transform t)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Vector3<float>::AsGLM(t.position));
		glm::vec3 rotation = Vector3<float>::AsGLM(t.rotation);

		glm::quat rot = glm::quat(glm::vec3(glm::radians(rotation.z), glm::radians(rotation.y), glm::radians(rotation.x)));

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), Vector3<float>::AsGLM(t.scale));
		return transform * glm::mat4(rot) * scale;
	}
	void Transform::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "Transform" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value; Serializer::Serialize(out, position);
		out << YAML::Key << "Rotation" << YAML::Value; Serializer::Serialize(out, rotation);
		out << YAML::Key << "Scale" << YAML::Value; Serializer::Serialize(out, scale);
		out << YAML::EndMap;
	}
	void Transform::DeserializeComponent(YAML::Node in) {

		this->position = Deserializer::Deserialize<Vector3<float>>(in["Position"]);
		this->rotation = Deserializer::Deserialize<Vector3<float>>(in["Rotation"]);
		this->scale = Deserializer::Deserialize<Vector3<float>>(in["Scale"]);
	}
}
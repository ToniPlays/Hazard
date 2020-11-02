#pragma once

#include <hzrpch.h>
#include "TransformComponent.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"


namespace Hazard {

	TransformComponent::TransformComponent() : Component("Transform") {
	}

	TransformComponent::~TransformComponent()
	{
		delete parent;
	}

	void TransformComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "Transform" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value; Serializer::Serialize(out, this->transform.position);
		out << YAML::Key << "Rotation" << YAML::Value; Serializer::Serialize(out, this->transform.rotation);
		out << YAML::Key << "Scale" << YAML::Value; Serializer::Serialize(out, this->transform.scale);
		out << YAML::EndMap;
	}
	void TransformComponent::DeserializeComponent(YAML::Node in) {

		this->transform.position = Deserializer::Deserialize<Vector3<float>>(in["Position"]);
		this->transform.rotation = Deserializer::Deserialize<Vector3<float>>(in["Rotation"]);
		this->transform.scale = Deserializer::Deserialize<Vector3<float>>(in["Scale"]);
	}

	
}
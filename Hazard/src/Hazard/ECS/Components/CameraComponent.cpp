#pragma once
#include <hzrpch.h>
#include "CameraComponent.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"

namespace Hazard {

	CameraComponent::CameraComponent() : Component("Camera Component") 
	{

	}
	CameraComponent::~CameraComponent()
	{

	}
	void CameraComponent::RecalculateViewMatrix(float w, float h)
	{
		float aspectX = w / h;
		glm::mat4 projection;
		if (type == CameraType::Orthographic) 
			projection = glm::ortho(-aspectX * FovSize, aspectX * FovSize, -FovSize, FovSize, clipping.x, clipping.y);
		else 
			projection = glm::perspective(glm::radians(FovSize), aspectX, clipping.x, clipping.y);

		viewProjection = projection * glm::inverse(Transform::AsMat4(*parent->GetComponent<Transform>()));
	}
	void CameraComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << (type == CameraType::Perspective ? "Perspective" : "Orthographic");
		out << YAML::Key << "Clipping" << YAML::Value; Serializer::Serialize(out, clipping);
		out << YAML::Key << "Size" << YAML::Value << FovSize;
		out << YAML::EndMap;
	}
	void CameraComponent::DeserializeComponent(YAML::Node in)
	{
		type = in["Type"].as<std::string>() == "Perspective" ? CameraType::Perspective : CameraType::Orthographic;
		clipping = Deserializer::Deserialize<Vector2<float>>(in["Clipping"]);
		FovSize = in["Size"].as<float>();
	}
}
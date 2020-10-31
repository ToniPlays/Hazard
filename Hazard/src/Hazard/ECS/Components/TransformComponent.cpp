#pragma once

#include <hzrpch.h>
#include "TransformComponent.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"


namespace Hazard {

	TransformComponent::TransformComponent(Transform* transform) : Component("Transform") {
		this->transform = transform;
	}

	void TransformComponent::SerializeComponent(YAML::Emitter& out)
	{
	}

	void TransformComponent::DeserializeComponent(YAML::Node in)
	{
	}

	
}
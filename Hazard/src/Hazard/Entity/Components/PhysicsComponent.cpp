#pragma once

#include "hzrpch.h"
#include "PhysicsComponent.h"

namespace Hazard::ECS {
	std::string BodyTypeToString(BodyType type)
	{
		switch (type)
		{
		case BodyType::Static:		return "Static";
		case BodyType::Kinematic:	return "Kinematic";
		case BodyType::Dynamic:	return "Dynamic";
		}
	}
	BodyType StringToBodyType(const std::string& value)
	{
		if (value == "Static")		return BodyType::Static;
		if(value == "Kinematic")	return BodyType::Kinematic;
		if(value == "Dynamic")		return BodyType::Dynamic;
	}
}

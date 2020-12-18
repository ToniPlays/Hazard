#pragma once
#include "Hazard/Core/Core.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {
	class Deserializer {
	public:
		//static bool DeserializeScene(const std::string& file);
		//static Entity* DeserializeEntity(YAML::Node ent);

		template<typename T>
		static T Deserialize(YAML::Node in);
	};
}
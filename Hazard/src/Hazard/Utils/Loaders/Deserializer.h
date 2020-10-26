#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Scene/Scene.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {
	class Deserializer {
	public:
		static bool DeserializeScene(const std::string& file, Scene& scene);
		static Entity* DeserializeEntity(YAML::Node ent);
	};
}
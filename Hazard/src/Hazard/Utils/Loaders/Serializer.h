#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace Hazard {
	class HAZARD_API Serializer {
	public:
		//static bool SerializeScene(const std::string& file, Scene& scene);
		//static void SerializeEntity(YAML::Emitter& out, Entity* entity);

		static void Serialize(YAML::Emitter& out, glm::vec2 value);
		static void Serialize(YAML::Emitter& out, glm::vec3 value);
		static void Serialize(YAML::Emitter& out, Color value);
		
	};
}
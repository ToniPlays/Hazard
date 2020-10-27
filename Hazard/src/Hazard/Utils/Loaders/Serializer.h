#pragma once

#include "Hazard/Modules/Scene/Scene.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {
	class HAZARD_API Serializer {
	public:
		static bool SerializeScene(const std::string& file, Scene& scene);
		static void SerializeEntity(YAML::Emitter& out, Entity* entity);

		static void Serialize(YAML::Emitter& out, Vector3<float> value);
		static void Serialize(YAML::Emitter& out, Color value);
		
	};
}
#pragma once

#include "../Entity.h"
#include <yaml-cpp/yaml.h>

namespace Hazard::ECS::Loader 
{
	class SceneDeserializer {
	public:

		static World* DeserializeEditor(const char* file);
		static World* DeserializeRuntime(const char* file) {};

		template<typename T>
		static void TryDeserializeComponent(const char* key, Entity entity, YAML::Node node);
		template<typename T>
		static void Deserialize(Entity entity, YAML::Node node);

	};
}
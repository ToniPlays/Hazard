#pragma once

#include "../Entity.h"
#include <yaml-cpp/yaml.h>

namespace Hazard::ECS::Loader 
{
	class WorldDeserializer {
	public:

		static bool DeserializeEditor(Ref<World>& world, const char* file);
		static bool DeserializeRuntime(Ref<World>& world, const char* file) { return false; };

		template<typename T>
		static void TryDeserializeComponent(const char* key, Entity entity, YAML::Node node);
		template<typename T>
		static void Deserialize(Entity entity, YAML::Node node);
	};
}
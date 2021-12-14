#pragma once

#include "yaml-cpp/yaml.h"

namespace Hazard 
{
	class YamlUtils {
	public:
		template<typename T>
		static void Serialize(YAML::Emitter& out, const std::string& key, T value);

		template<typename T>
		static void Deserialize(YAML::Node node, const std::string& key, T& value, T defaultValue);

		template<typename T>
		static void Map(YAML::Emitter& out, const std::string& key, T fn) 
		{
			out << YAML::Key << key << YAML::Value << YAML::BeginMap;
			fn();
			out << YAML::EndMap;
		}

		template<typename T>
		static void Sequence(YAML::Emitter& out, const std::string& key, T fn)
		{
			out << YAML::Key << key << YAML::Value << YAML::BeginSeq;
			fn();
			out << YAML::EndSeq;
		}
	};
}
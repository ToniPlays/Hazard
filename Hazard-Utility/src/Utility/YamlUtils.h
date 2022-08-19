#pragma once

#include "Color.h"

#include "yaml-cpp/yaml.h"
#include <glm/glm.hpp>
#include <filesystem>

#include "UID.h"


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}


class YamlUtils {
public:
	template<typename T>
	static void Serialize(YAML::Emitter& out, const std::string& key, T value) {
		out << YAML::Key << key << YAML::Value << value;
	}

	template<typename T>
	static void Deserialize(YAML::Node node, const std::string& key, T& value, T defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<T>();
	}

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

	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, bool value)
	{
		out << YAML::Key << key << YAML::Value << (value ? "True" : "False");
	}
	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, std::filesystem::path value)
	{
		out << YAML::Key << key << YAML::Value << value.string();
	}

	//------------Vectors//------------
	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, glm::vec2 value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
	}
	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, glm::vec3 value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
	}
	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, glm::vec4 value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
	}
	template<>
	static void Serialize(YAML::Emitter& out, const std::string& key, Color value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.r << value.g << value.b << value.a << YAML::EndSeq;
	}


	template<>
	static void Deserialize(YAML::Node node, const std::string& key, bool& value, bool defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<std::string>() == "True";
	}
	template<>
	static void Deserialize(YAML::Node node, const std::string& key, Color& value, Color defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = Color::FromGLM(node[key].as<glm::vec4>());
	}
	template<>
	static void Deserialize(YAML::Node node, const std::string& key, std::filesystem::path& value, std::filesystem::path defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<std::string>();
	}
};

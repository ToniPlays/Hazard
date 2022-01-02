
#include <hzrpch.h>
#include "YamlUtils.h"
#include "Hazard/Math/Color.h"

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

namespace Hazard 
{
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, bool value)
	{
		out << YAML::Key << key << YAML::Value << (value ? "True" : "False");
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, const char* value)
	{
		out << YAML::Key << key << YAML::Value << value;
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, std::string value) 
	{
		out << YAML::Key << key << YAML::Value << value;
	}

	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, uint32_t value)
	{
		out << YAML::Key << key << YAML::Value << value;
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, size_t value)
	{
		out << YAML::Key << key << YAML::Value << value;
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, float value)
	{
		out << YAML::Key << key << YAML::Value << value;
	}

	//------------Vectors//------------
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, glm::vec2 value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, glm::vec3 value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
	}
	template<>
	void YamlUtils::Serialize(YAML::Emitter& out, const std::string& key, Color value)
	{
		out << YAML::Key << key << YAML::Value;
		out << YAML::Flow;
		out << YAML::BeginSeq << value.r << value.g << value.b << value.a << YAML::EndSeq;
	}




	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, bool& value, bool defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<std::string>() == "True";
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, std::string& value, std::string defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<std::string>();
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, uint32_t& value,  uint32_t defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<uint32_t>();
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, float& value, float defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<float>();
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, glm::vec2& value, glm::vec2 defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<glm::vec2>();
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, glm::vec3& value, glm::vec3 defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = node[key].as<glm::vec3>();
	}
	template<>
	void YamlUtils::Deserialize(YAML::Node node, const std::string& key, Color& value, Color defaultValue) {
		if (!node[key]) value = defaultValue;
		else value = Color::FromGLM(node[key].as<glm::vec4>());
	}
}

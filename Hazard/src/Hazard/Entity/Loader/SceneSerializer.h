#pragma once

#include "SceneHandler.h"
#include "../World.h"
#include "../Entity.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

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


namespace Hazard::ECS::Loader 
{
	template<typename T>
	static void Convert(YAML::Emitter& out, T v) {
		static_assert(false);
	};

	template<>
	static void Convert(YAML::Emitter& out, glm::vec2 v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	};
	template<>
	static void Convert(YAML::Emitter& out, glm::vec3 v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	};
	template<>
	static void Convert(YAML::Emitter& out, glm::vec4 v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	};
	template<>
	static void Convert(YAML::Emitter& out, Color v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.r << v.g << v.b << v.a << YAML::EndSeq;
	};
	template<>
	static void Convert(YAML::Emitter& out, bool v) {
		out << (v ? "True" : "False");
	};

	class SceneSerializer {
	public:

		template<typename T>
		static void TrySerializeEditor(Entity entity, YAML::Emitter& out)
		{
			if (entity.HasComponent<T>()) {
				SerializeComponentEditor(entity, entity.GetComponent<T>(), out);
			}
		}
		static bool SerializeEditor(const char* file, World& world);
		static void SerializeEntityEditor(Entity entity, YAML::Emitter& out);
		template<typename T>
		static void SerializeComponentEditor(Entity entity, T& component, YAML::Emitter& out);

		static bool SerializeRuntime(const char* file, Scene& scene);
	};
}
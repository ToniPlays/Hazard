#pragma once

#include "SceneHandler.h"
#include "Scene.h"
#include "Entity.h"
#include "yaml-cpp/yaml.h"

#include <glm/glm.hpp>

namespace YAML 
{

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs) {
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

namespace Hazard::ECS::Loader {


	class SceneDeserializer {
	public:

		//Deserialize Editor
		static Scene* DeserializeEditor(const char* file);
		template<typename T>
		static void Deserialize(Entity entity, YAML::Node node);

		template<>
		static void Deserialize<TagComponent>(Entity entity, YAML::Node node) {

			if (!node["TagComponent"]) return;
			auto comp = node["TagComponent"];

			std::string tag = comp["Tag"].as<std::string>();
			entity.GetComponent<TagComponent>().tag = tag;
		};

		template<>
		static void Deserialize<TransformComponent>(Entity entity, YAML::Node node) {


			if (!node["TransformComponent"]) return;
				auto comp = node["TransformComponent"];

			auto& c = entity.GetComponent<TransformComponent>();

			c.Translation = comp["Translation"].as<glm::vec3>();
			c.Rotation = comp["Rotation"].as<glm::vec3>();
			c.Scale = comp["Scale"].as<glm::vec3>();
		};
		template<>
		static void Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node node) {


			if (!node["SpriteRendererComponent"]) return;
			auto comp = node["SpriteRendererComponent"];

			entity.AddComponent<SpriteRendererComponent>().tint = Color::FromGLM(comp["Tint"].as<glm::vec4>());

		};

		//Deserialize runtime
		static Scene* DeserializeRuntime(const char* file);

	};
}
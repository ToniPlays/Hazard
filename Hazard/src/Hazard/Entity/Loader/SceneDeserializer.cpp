#pragma once

#include <hzrpch.h>
#include "SceneDeserializer.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

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
	World* SceneDeserializer::DeserializeEditor(const char* file)
	{
		YAML::Node root = YAML::LoadFile(file);
		World* world = new World(file);

		//Set scene name
		if (!root["Scene"]) return world;
		world->SetName(root["Scene"].as<std::string>());

		//Loop entities
		auto entities = root["Entities"];
		if (entities) {

			for (int i = entities.size() - 1; i >= 0; i--) {

				auto node = entities[i];
				uint64_t uuid = node["Entity"].as<uint64_t>();

				Entity entity = world->CreateEntity("");
				//Deserialize components
				TryDeserializeComponent<TagComponent>("TagComponent", entity, node);
				TryDeserializeComponent<TransformComponent>("TransformComponent", entity, node);
				TryDeserializeComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, node);
				TryDeserializeComponent<CameraComponent>("CameraComponent", entity, node);
				TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);
				TryDeserializeComponent<VisualScriptComponent>("VisualScriptComponent", entity, node);
				TryDeserializeComponent<MeshComponent>("MeshComponent", entity, node);
			}
		}
		return world;
	}
	template<typename T>
	void SceneDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node) {
		if (node[key]) {
			Deserialize<T>(entity, node[key]);
		}
	}
	template<typename T>
	void SceneDeserializer::Deserialize(Entity entity, YAML::Node node) {
		static_assert(false);
	}

	template<>
	static void SceneDeserializer::Deserialize<TagComponent>(Entity entity, YAML::Node comp) {
		std::string tag = comp["Tag"].as<std::string>();
		entity.GetComponent<TagComponent>().m_Tag = tag;
	};

	template<>
	static void SceneDeserializer::Deserialize<TransformComponent>(Entity entity, YAML::Node comp) {
		auto& c = entity.GetComponent<TransformComponent>();

		c.m_Translation = comp["Translation"].as<glm::vec3>();
		c.m_Rotation = comp["Rotation"].as<glm::vec3>();
		c.m_Scale = comp["Scale"].as<glm::vec3>();
	};
	template<>
	static void SceneDeserializer::Deserialize<SpriteRendererComponent>(Entity entity, YAML::Node comp) {
		auto& component = entity.AddComponent<SpriteRendererComponent>();
		component.m_Tint = Color::FromGLM(comp["Tint"].as<glm::vec4>());

		if (comp["Texture"]) {

			using namespace Hazard::Rendering;

			Texture2DCreateInfo createInfo;
			createInfo.filename = comp["Texture"].as<std::string>().c_str();
			component.m_Texture = RenderUtils::Create<Texture2D>(createInfo);
		}
	};
	template<>
	static void SceneDeserializer::Deserialize<CameraComponent>(Entity entity, YAML::Node comp) {

		auto& c = entity.AddComponent<CameraComponent>();
		c.SetProjection(comp["Projection"].as<std::string>() == "Orthographic" ? Projection::Orthographic : Projection::Perspective);
		c.SetFov(comp["Fov"].as<float>());
	};

	template<>
	static void SceneDeserializer::Deserialize<ScriptComponent>(Entity entity, YAML::Node comp) {
		std::string moduleName = comp["ModuleName"].as<std::string>();
		entity.AddComponentWithCallback<ScriptComponent>([&](ScriptComponent& comp) {
			comp.m_ModuleName = moduleName;
			});
	};
	template<>
	static void SceneDeserializer::Deserialize<VisualScriptComponent>(Entity entity, YAML::Node comp) {
		std::string filename = comp["ModuleName"].as<std::string>();
		entity.AddComponentWithCallback<VisualScriptComponent>([&](VisualScriptComponent& comp) {
			comp.m_Filename = filename;
			});
	};
	template<>
	static void SceneDeserializer::Deserialize<MeshComponent>(Entity entity, YAML::Node comp) {
		auto& c = entity.AddComponent<MeshComponent>();
		c.m_Mesh = Rendering::MeshFactory::LoadMesh(comp["File"].as<std::string>());
	};
}
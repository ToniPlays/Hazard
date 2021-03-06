#pragma once

#include <hzrpch.h>
#include "SceneSerializer.h"


namespace Hazard::ECS::Loader {

	Scene* SceneSerializer::DeserializeEditor(const char* file)
	{
		YAML::Node root = YAML::LoadFile(file);
		Scene* scene = new Scene(file);

		if (!root["Scene"]) return scene;

		scene->SetName(root["Scene"].as<std::string>());

		auto entities = root["Entities"];
		if (entities) {
			for (auto node : root["Entities"]) {
				uint64_t uuid = node["Entity"].as<uint64_t>();

				Entity entity = scene->CreateEntity("");

				Deserialize<TagComponent>(entity, node);
				Deserialize<TransformComponent>(entity, node);
				Deserialize<SpriteRendererComponent>(entity, node);
				Deserialize<CameraComponent>(entity, node);
			}
		}

		return scene;
	}

	Scene* SceneSerializer::DeserializeRuntime(const char* file)
	{
		HZR_ERROR("Runtime serialization TODO");
		return nullptr;
	}
	bool SceneSerializer::SerializeEditor(const char* file, Scene& scene)
	{

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene.GetName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		scene.GetSceneRegistry().each([&](auto entityID) {
			Entity entity{ entityID, &scene };
			if (!entity) {
				return;
			}
			SerializeEntityEditor(entity, out);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(file);
		fout << out.c_str();

		return true;
	}
	void SceneSerializer::SerializeEntityEditor(Entity entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "43243234";

		SerializeComponentEditor<TagComponent>(entity, out);
		SerializeComponentEditor<TransformComponent>(entity, out);
		SerializeComponentEditor<SpriteRendererComponent>(entity, out);
		SerializeComponentEditor<CameraComponent>(entity, out);

		out << YAML::EndMap;
	}

	bool SceneSerializer::SerializeRuntime(const char* file, Scene& scene)
	{
		return false;
	}
}
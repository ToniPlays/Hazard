#pragma once

#include <hzrpch.h>
#include "SceneDeserializer.h"


namespace Hazard::ECS::Loader {

	Scene* SceneDeserializer::DeserializeEditor(const char* file)
	{
		YAML::Node root = YAML::LoadFile(file);

		if (!root["Scene_name"]) return new Scene("Empty scene");

		Scene* scene = new Scene(root["Scene_name"].as<std::string>());

		auto entities = root["Entities"];
		if (entities) {
			for (auto node : root["Entities"]) {
				uint64_t uuid = node["Entity"].as<uint64_t>();

				Entity entity = scene->CreateEntity("");
				Deserialize<TagComponent>(entity, node);
				Deserialize<TransformComponent>(entity, node);
				Deserialize<SpriteRendererComponent>(entity, node);
			}
		}

		return scene;
	}

	Scene* SceneDeserializer::DeserializeRuntime(const char* file)
	{
		HZR_ERROR("Runtime serialization TODO");
		return nullptr;
	}
}
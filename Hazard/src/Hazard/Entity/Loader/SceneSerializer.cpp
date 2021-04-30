#pragma once

#include <hzrpch.h>
#include "SceneSerializer.h"


namespace Hazard::ECS::Loader {

	//Deserialize scene editor
	World* SceneSerializer::DeserializeEditor(const char* file)
	{
		YAML::Node root = YAML::LoadFile(file);
		World* world = new World(file);

		//Set scene name
		if (!root["Scene"]) return world;
		world->SetName(root["Scene"].as<std::string>());

		//Loop entities
		auto entities = root["Entities"];
		if (entities) {
			for (auto node : root["Entities"]) {
				uint64_t uuid = node["Entity"].as<uint64_t>();

				Entity entity = world->CreateEntity("");
				//Deserialize components
				TryDeserialize<TagComponent>("TagComponent", entity, node);
				TryDeserialize<TransformComponent>("TransformComponent", entity, node);
				TryDeserialize<SpriteRendererComponent>("SpriteRendererComponent", entity, node);
				TryDeserialize<CameraComponent>("CameraComponent", entity, node);
				TryDeserialize<ScriptComponent>("ScriptComponent", entity, node);
				TryDeserialize<VisualScriptComponent>("VisualScriptComponent", entity, node);
				TryDeserialize<MeshComponent>("MeshComponent", entity, node);
			}
		}
		return world;
	}
	//Deserialize runtime file
	Scene* SceneSerializer::DeserializeRuntime(const char* file)
	{
		HZR_ERROR("Runtime serialization TODO");
		return nullptr;
	}

	bool SceneSerializer::SerializeEditor(const char* file, World& world)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << (world.GetName() != "" ? world.GetName() : "Untitled scene");
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		world.GetWorldRegistry().each([&](auto entityID) {
			Entity entity{ entityID, &world };
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
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		TryDeserializeEditor<TagComponent>(entity, out);
		TryDeserializeEditor<TransformComponent>(entity, out);
		TryDeserializeEditor<SpriteRendererComponent>(entity, out);
		TryDeserializeEditor<CameraComponent>(entity, out);
		TryDeserializeEditor<ScriptComponent>(entity, out);
		TryDeserializeEditor<MeshComponent>(entity, out);

		out << YAML::EndMap;
	}

	bool SceneSerializer::SerializeRuntime(const char* file, Scene& scene)
	{
		return false;
	}
}
#pragma once

#include <hzrpch.h>
#include "WorldDeserializer.h"
#include "Hazard/Utils/YamlUtils.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::ECS::Loader
{
	bool WorldDeserializer::DeserializeEditor(Ref<World>& world, const char* file)
	{
		YAML::Node root = YAML::LoadFile(file);

		if (!world)
			world = Ref<World>::Create(file);

		//Set scene name
		if (!root["World"]) {
			return false;
		}

		world->SetName(root["World"].as<std::string>());

		//Loop entities
		auto entities = root["Entities"];
		if (entities) {

			for (size_t i = entities.size() - 1; i >= 0; i--) {

				auto node = entities[i];
				Entity entity = world->CreateEntity("");
				//Deserialize components
				TryDeserializeComponent<TagComponent>("TagComponent", entity, node);
				TryDeserializeComponent<TransformComponent>("TransformComponent", entity, node);
				TryDeserializeComponent<CameraComponent>("CameraComponent", entity, node);

				TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);
				TryDeserializeComponent<VisualScriptComponent>("VisualScriptComponent", entity, node);

				TryDeserializeComponent<SkyLightComponent>("SkyLightComponent", entity, node);
				TryDeserializeComponent<DirectionalLightComponent>("DirectionalLightComponent", entity, node);
				TryDeserializeComponent<PointLightComponent>("PointLightComponent", entity, node);

				TryDeserializeComponent<AudioSourceComponent>("AudioSourceComponent", entity, node);

				TryDeserializeComponent<MeshComponent>("MeshComponent", entity, node);
				TryDeserializeComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, node);

				TryDeserializeComponent<Rigidbody2DComponent>("Rigidbody2DComponent", entity, node);
				TryDeserializeComponent<BoxCollider2DComponent>("BoxCollider2DComponent", entity, node);
				TryDeserializeComponent<CircleCollider2DComponent>("CircleCollider2DComponent", entity, node);

				TryDeserializeComponent<BatchComponent>("BatchComponent", entity, node);
			}
		}
		return true;
	}
	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node) {
		if (node[key]) {
			Deserialize<T>(entity, node[key]);
		}
	}
}

#pragma once
#include <hzrpch.h>
#include "Deserializer.h"
#include "Hazard/ECS/Components/TransformComponent.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"
#include "Hazard/ECS/Components/CameraComponent.h"
#include "Hazard/ECS/Components/BatchRendererComponent.h"

namespace Hazard {

	void RunPostDeserialize(Entity* entity) {
		for (Component* c : entity->GetComponents()) c->PostDeserialize();
		for (Entity* child : entity->GetChildEntities()) RunPostDeserialize(child);
	}

	bool Deserializer::DeserializeScene(const std::string& file, Scene& scene) {
		std::ifstream stream(file);
		std::stringstream ss;

		ss << stream.rdbuf();
		YAML::Node root = YAML::Load(ss.str());

		if (!root["HazardScene"]) 
			return false;

		scene.SetSceneName(root["HazardScene"].as<std::string>());
		if (!root["Entities"])
			return false;
		
		for (YAML::Node ent : root["Entities"]) {
			Entity* entity = Deserializer::DeserializeEntity(ent);
			scene.AddEntity(entity);
		}
		
		for (Entity* entity : scene.GetChildEntities())
			RunPostDeserialize(entity);

		return true;
	}
	Entity* Deserializer::DeserializeEntity(YAML::Node ent) {

		Entity* entity = new Entity(ent["Name"].as<std::string>());
		
		for (const auto& kv : ent["Components"]) {
			Component* component = nullptr;
			std::string key = kv.first.as<std::string>();
			YAML::Node value = kv.second.as<YAML::Node>();

			if (key == "Transform") component = new Transform();
			if (key == "SpriteRenderer") component = new SpriteRenderer();
			if (key == "CameraComponent") component = new CameraComponent();
			if (key == "BatchRendererComponent") component = new BatchRendererComponent();

			if (component == nullptr) continue;

			component->DeserializeComponent(value);
			entity->AddComponent(component);
		}
		return entity;
	}
	template<>
	Vector2<float>Deserializer::Deserialize(YAML::Node in) {
		return { in[0].as<float>(), in[1].as<float>() };
	}
	template<>
	Vector3<float>Deserializer::Deserialize(YAML::Node in) {
		return { in[0].as<float>(), in[1].as<float>(), in[2].as<float>() };
	}
	template<>
	Color Deserializer::Deserialize(YAML::Node in) {
		Color color;
		color.r = in[0].as<float>();
		color.g = in[1].as<float>();
		color.b = in[2].as<float>();
		color.a = in[3].as<float>();
		return color;
	}
}
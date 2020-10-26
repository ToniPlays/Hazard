#pragma once
#include <hzrpch.h>
#include "Deserializer.h"

namespace Hazard {

	bool Deserializer::DeserializeScene(const std::string& file, Scene& scene) {
		std::ifstream stream(file);
		std::stringstream ss;

		ss << stream.rdbuf();
		YAML::Node root = YAML::Load(ss.str());

		if (!root["Hazard scene"]) 
			return false;

		scene.SetSceneName(root["Hazard scene"].as<std::string>());

		if (!root["Entities"])
			return false;
		
		for (YAML::Node ent : root["Entities"]) {
			Entity* entity = Deserializer::DeserializeEntity(ent);
			scene.AddEntity(entity);
		}

		return true;
	}
	Entity* Deserializer::DeserializeEntity(YAML::Node ent) {

		Entity* entity = new Entity(ent["Name"].as<std::string>());
		
		return entity;
	}
}
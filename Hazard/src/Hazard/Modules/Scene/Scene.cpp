#pragma once

#include <hzrpch.h>
#include "Scene.h"

namespace Hazard {
	
	Scene::Scene(std::string name) : Entity(name) {};
	void Scene::AddEntity(std::string name, Entity* entity) {
		if (entity == nullptr);
			entity = new Entity(name);
		entities.push_back(entity);
	}
	void Scene::RemoveEntity(Entity* entity) {
		auto iterator = std::find(entities.begin(), entities.end(), entity);
		if (iterator != entities.end()) {
			entity->OnDestroy();
			entities.erase(iterator);
		}
	}
}
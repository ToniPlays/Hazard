#pragma once

#include <hzrpch.h>
#include "Scene.h"

namespace Hazard {
	Scene* Scene::instance;
	
	Scene::Scene(std::string scene) : name(scene), objects(std::vector<GameObject*>())
	{
		Scene::instance = this;
	}

	void Scene::AddGameObject(GameObject* object)
	{
		objects.push_back(object);
	}
}
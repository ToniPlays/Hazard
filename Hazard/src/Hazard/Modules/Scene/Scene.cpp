#pragma once

#include <hzrpch.h>
#include "Scene.h"

namespace Hazard {
	
	Scene::Scene(std::string name) : Entity(name) {};

	void Scene::Flush()
	{
		for (Entity* entity : GetChildEntities()) {
			RemoveEntity(entity);
			delete entity;
		}
	}
}
#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Hazard/ECS/Components/Transform.h"
#include "Hazard/ECS/Components/CameraComponent.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"

namespace Hazard {
	
	Scene::Scene(std::string name) : Entity(name) {};

	void Scene::Flush()
	{
		HZR_CORE_INFO("Flushing scene " + name);
		for (Entity* entity : GetChildEntities()) {
			RemoveEntity(entity);
			delete entity;
		}
	}
	void Scene::OnComponentAdded(Component* component) {

		if (dynamic_cast<CameraComponent*>(component)) {
			this->sceneCamera = (CameraComponent*)component;
		}
	}

	void Scene::OnComponentRemoved(Component* component)
	{
		if (dynamic_cast<CameraComponent*>(component)) {
			this->sceneCamera = nullptr;
		}
	}
	
}
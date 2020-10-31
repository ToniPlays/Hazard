#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Hazard/ECS/Components/TransformComponent.h"
#include "Hazard/ECS/Components/CameraComponent.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"

namespace Hazard {
	
	Scene::Scene(char* name) : Entity(name) {};

	void Scene::Awake()
	{
		for (Entity* entity : GetChildEntities()) {
			entity->Awake();
		}
	}

	void Scene::Flush()
	{
		HZR_CORE_INFO("Flushing scene " + std::string(name));
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
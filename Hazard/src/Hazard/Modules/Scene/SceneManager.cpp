#pragma once
#include <hzrpch.h>
#include "SceneManager.h"
#include "Hazard/Utils/Loaders/Deserializer.h"

namespace Hazard {
	SceneManager::SceneManager(Scene* scene) : Module("SceneManager") {
		if (scene == nullptr)
			LoadEmptyScene();
		else ActivateScene(scene);
	}
	void SceneManager::LoadScene(std::string& path) {
		Scene* scene = new Scene();
		if (Deserializer::DeserializeScene(path, *scene)) {
			ActivateScene(scene);
		}
	}
	void SceneManager::ActivateScene(Scene* scene) {
		if(activeScene != nullptr)
			activeScene->Flush();
		activeScene = scene;

		for (Entity* entity : activeScene->GetChildEntities()) HZR_CORE_INFO(entity->name);
	}
	void SceneManager::LoadEmptyScene() {

		Scene* empty = new Scene("Empty scene");
		empty->AddEntity(new Entity("Sky light"));
		empty->AddEntity(new Entity("Camera entity"));

		ActivateScene(empty);
	}
}

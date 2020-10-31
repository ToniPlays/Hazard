#pragma once
#include <hzrpch.h>
#include "SceneManager.h"
#include "Hazard/Utils/Loaders/Deserializer.h"

namespace Hazard {
	SceneManager::SceneManager() : Module("SceneManager")
	{
		activeScene = new Scene("Empty scene");
	}
	SceneManager::SceneManager(Scene* scene) : Module("SceneManager") {
		if (scene == nullptr)
			LoadEmptyScene();
		else ActivateScene(scene);
	}
	void SceneManager::LoadScene(std::string& path) {
		Scene* tempScene = GetActiveScene();
		activeScene = new Scene();
		HZR_CORE_INFO("Loading scene");
		if (Deserializer::DeserializeScene(path, *activeScene)) {
			Scene* loadedScene = activeScene;
			activeScene = tempScene;
			ActivateScene(loadedScene);
			return;
		}
		activeScene = tempScene;
	}
	void SceneManager::ActivateScene(Scene* scene) {
		if(activeScene != nullptr)
			activeScene->Flush();
		activeScene = scene;
		activeScene->Awake();
		HZR_CORE_INFO("Activating scene " + scene->GetName());
		
	}
	void SceneManager::LoadEmptyScene() {

		Scene* empty = new Scene("Empty scene");
		empty->AddEntity(new Entity("Sky light"));
		empty->AddEntity(new Entity("Camera entity"));

		ActivateScene(empty);
	}
}

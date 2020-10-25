#pragma once
#include <hzrpch.h>
#include "SceneManager.h"

namespace Hazard {
	SceneManager::SceneManager(Scene* scene) : Module("SceneManager") {
		if (scene == nullptr) 
			LoadEmptyScene();
		else LoadScene(scene);
	}
	void SceneManager::LoadScene(Scene* scene) {
		LoadEmptyScene();
	}
	void SceneManager::LoadEmptyScene() {
		activeScene = new Scene("Empty scene");
		activeScene->AddEntity("Sky light");
		activeScene->AddEntity("CameraEntity");
	}
}

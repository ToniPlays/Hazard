#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Entity.h"
#include "Hazard/ECS/Components/CameraComponent.h"
#include <vector>

namespace Hazard {

	class HAZARD_API Scene : public Entity {
		friend class RenderEngine;
	public:

		Scene(std::string name = "Empty scene");
		~Scene() {};

		void Flush();
		void OnComponentAdded(Component* component);
		void OnComponentRemoved(Component* component);
		std::string GetName() { return name; }
		void SetSceneName(std::string name) { this->name = name; }
		CameraComponent* sceneCamera = nullptr;

	private:
	};
}
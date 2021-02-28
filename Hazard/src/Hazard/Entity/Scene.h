#pragma once

#include "Component.h"
#include "entt.hpp"

namespace Hazard::ECS {
	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene(std::string file);
		~Scene();

		void Render();
		void Flush();

		Entity CreateEntity(const char* name);
		void DestroyEntity(Entity entity);

		CameraComponent& GetSceneCamera() { return sceneCamera; }

		entt::registry& GetSceneRegistry() { return registry; }
		std::string& GetSceneFile() { return file; }

	private:
		entt::registry registry;
		std::string file;

		CameraComponent sceneCamera;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);

	};
}
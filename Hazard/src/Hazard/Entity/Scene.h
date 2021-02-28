#pragma once

#include "Component.h"
#include "entt.hpp"

namespace Hazard::ECS {
	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene();
		Scene(const char* file);
		~Scene();

		void Render();
		void Flush();

		Entity CreateEntity(const char* name);
		void DestroyEntity(Entity entity);

		CameraComponent& GetSceneCamera() { return sceneCamera; }

		entt::registry& GetSceneRegistry() { return registry; }
		const char* GetSceneFile() { return file; }

	private:
		entt::registry registry;
		const char* file;

		CameraComponent sceneCamera;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);

	};
}
#pragma once

#include <entt.hpp>
#include "Component.h"

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

		void SetName(std::string name) { this->name = name.c_str(); }

		std::tuple<bool, Camera> GetSceneCamera();
		entt::registry& GetSceneRegistry() { return registry; }

		std::string& GetSceneFile() { return file; }
		std::string& GetName() { return name; }
		
	private:
		entt::registry registry;
		std::string name;
		std::string file;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);

	};
}
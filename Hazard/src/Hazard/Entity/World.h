#pragma once

#include <entt.hpp>
#include "Component.h"

namespace Hazard::ECS {
	class Entity;

	class World {
		friend class Entity;
	public:
		World(std::string file);
		~World();

		void Flush();

		void RenderAll();

		Entity CreateEntity(const char* name);
		Entity GetEntity(entt::entity id);
		void DestroyEntity(Entity entity);

		void SetName(std::string name) { this->name = name.c_str(); }

		std::tuple<bool, CameraComponent*, TransformComponent*> GetWorldCamera();
		entt::registry& GetWorldRegistry() { return registry; }

		std::string& GetWorldFile() { return file; }
		std::string& GetName() { return name; }

		template<typename T>
		std::vector<std::tuple<uint32_t, T>> FindEntitiesWith() {
			std::vector<std::tuple<uint32_t, T>> result;
			auto view = registry.view<T>();
			for (auto entity : view) {
				auto comp = view.get<T>(entity);
				result.push_back(std::tuple((uint32_t)entity, comp));
			}
			return result;
		}
		
	private:
		entt::registry registry;
		std::string name;
		std::string file;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity& entity, T& component);

	};
}
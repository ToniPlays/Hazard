#pragma once

#include "entt.hpp"

namespace Hazard {

	namespace ECS {

		class Entity;

		class Scene {

		public:
			Scene();
			Scene(const char* _file);

			Entity CreateEntity(const std::string& name = "Empty entity");

			void DestroyEntity(Entity entity);
			const char* GetFile() { return file; }
			entt::registry& GetRegistry() { return registry; }
			
			~Scene();



		private:
			template<typename T>
			void OnComponentAdded(Entity entity, T& component);

		private:
			entt::registry registry;
			const char* file;

			friend class Entity;
			friend class Hierarchy;
		};
	}
}
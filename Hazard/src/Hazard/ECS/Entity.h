#pragma once

#include <hzrpch.h>

#include "entt.hpp"
#include "Scene.h"

namespace Hazard {
	namespace ECS {
		class Entity {

		public:
			Entity() = default;
			Entity(entt::entity entityHandle, Scene* _scene);
			Entity(const Entity& other) = default;

			template<typename T, typename... Args>
			T& AddComponent(Args&&... args) {
				//HZR_ASSERT(HasComponent<T>(), "Entity has component already!");
				T& component = scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
				scene->OnComponentAdded<T>(*this, component);
				return component;
			}
			template<typename T>
			T& GetComponent() {
				//HZR_ASSERT(!HasComponent<T>(), "Entity does not have component!");
				return scene->registry.get<T>(handle);
			}
			template<typename T>
			bool HasComponent() {
				return scene->registry.has<T>(handle);
			}

			template<typename T>
			void RemoveComponent() {
				scene->registry.remove<T>(handle);
			}

			operator bool() const { return handle != entt::null; }
			operator entt::entity() const { return handle; }
			operator uint32_t() const { return (uint32_t)handle; }

			bool operator ==(const Entity& other) {
				return handle == other.handle && scene == other.scene;
			}
			bool operator != (const Entity& other) {
				return !(*this == other);
			}

		private:
			entt::entity handle{ entt::null };
			Scene* scene = nullptr;

			friend class Scene;
		};
	}
}
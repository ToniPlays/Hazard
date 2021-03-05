#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Hazard::ECS {

	class Entity {
		friend class Scene;
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		Scene& GetScene() { return *scene; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			T& component = scene->registry.emplace<T>(*this, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent() {
			scene->registry.remove<T>(handle);
		}

		template<typename T>
		T& GetComponent() {
			return scene->registry.get<T>(handle);
		}

		template<typename T>
		bool HasComponent() {
			return scene->registry.has<T>(handle);
		}

		bool IsValid() { 
			if (scene == nullptr) return false;
			return scene->GetSceneRegistry().valid(handle); 
		}

		bool IsVisible() { return visible; }

	public:
		operator bool() const { return handle != entt::null; }
		operator entt::entity() const { return handle; }
		operator uint32_t() const { return (uint32_t)handle; }

		bool operator ==(const Entity& other) {
			return handle == other.handle && scene == other.scene;
		}
		bool operator !=(const Entity& other) {
			return !(*this == other);
		}

	private:
		entt::entity handle{ entt::null };
		Scene* scene = nullptr;

		bool visible = true;
	};
}
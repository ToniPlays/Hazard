#pragma once

#include "World.h"
#include "entt.hpp"

namespace Hazard::ECS {

	class Entity {
		friend class Scene;
	public:
		Entity() = default;
		Entity(entt::entity handle, World* world);
		Entity(const Entity& other) = default;

		World& GetWorld() { return *world; }

		//Add component to entity
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>()) {
				HZR_CORE_WARN("Entity already has component");
			}
			T& component = world->registry.emplace<T>(*this, std::forward<Args>(args)...);
			world->OnComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T, typename... Args>
		void AddComponent(T& component, Args&&... args) {
			if (HasComponent<T>()) {
				HZR_CORE_WARN("Entity already has component");
			}
			world->registry.emplace<T>(*this, std::forward<Args>(args)...);
			world->OnComponentAdded<T>(*this, component);
		}
		//Remove component from entity
		template<typename T>
		void RemoveComponent() {
			world->OnComponentRemoved<T>(*this, GetComponent<T>());
			world->registry.remove<T>(handle);
		}
		template<>
		void RemoveComponent<TagComponent>() {};
		template<>
		void RemoveComponent<TransformComponent>() {};

		template<typename T>
		T& GetComponent() {
			return world->registry.get<T>(handle);
		}

		template<typename T>
		bool HasComponent() {
			return world->registry.has<T>(handle);
		}

		bool IsValid() { 
			if (world == nullptr) return false;
			return world->GetWorldRegistry().valid(handle);
		}

		bool IsVisible() { return GetComponent<TagComponent>().visible; }
		void SetVisible(bool visible) { GetComponent<TagComponent>().visible = visible; }

		TransformComponent& GetTransform() { return GetComponent<TransformComponent>(); }
		TagComponent& GetTag() { return GetComponent<TagComponent>(); }

	public:
		operator bool() const { return handle != entt::null; }
		operator entt::entity() const { return handle; }
		operator uint32_t() const { return (uint32_t)handle; }

		bool operator ==(const Entity& other) {
			return handle == other.handle && world == other.world;
		}
		bool operator !=(const Entity& other) {
			return !(*this == other);
		}

	private:
		entt::entity handle { entt::null };
		World* world = nullptr;
	};
}
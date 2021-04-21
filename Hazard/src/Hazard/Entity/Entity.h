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

		World& GetWorld() { return *m_World; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>()) {
				HZR_CORE_WARN("Entity already has component");
			}
			T& component = m_World->m_Registry.emplace<T>(*this, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T, typename... Args>
		void AddComponent(T& component, Args&&... args) {
			if (HasComponent<T>()) {
				HZR_CORE_WARN("Entity already has component");
			}
			m_World->m_Registry.emplace<T>(*this, std::forward<Args>(args)...);
			m_World->OnComponentAdded<T>(*this, component);
		}
		template<typename T>
		void RemoveComponent() {
			m_World->OnComponentRemoved<T>(*this, GetComponent<T>());
			m_World->m_Registry.remove<T>(m_Handle);
		}
		template<>
		void RemoveComponent<TagComponent>() {};
		template<>
		void RemoveComponent<TransformComponent>() {};

		template<typename T>
		T& GetComponent() {
			return m_World->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent() { return m_World->m_Registry.has<T>(m_Handle); }

		bool IsValid() { 
			if (m_World == nullptr) 
				return false;

			return m_World->GetWorldRegistry().valid(m_Handle);
		}
		bool IsVisible() { return GetComponent<TagComponent>().m_Visible; }
		void SetVisible(bool visible) { GetComponent<TagComponent>().m_Visible = visible; }

		TransformComponent& GetTransform() { return GetComponent<TransformComponent>(); }
		TagComponent& GetTag() { return GetComponent<TagComponent>(); }

	public:
		operator bool() const { return m_Handle != entt::null; }
		operator entt::entity() const { return m_Handle; }
		operator uint32_t() const { return (uint32_t)m_Handle; }

		bool operator ==(const Entity& other) {
			return m_Handle == other.m_Handle && m_World == other.m_World;
		}
		bool operator !=(const Entity& other) {
			return !(*this == other);
		}

	private:
		entt::entity m_Handle { entt::null };
		World* m_World = nullptr;
	};
}
#pragma once

#include <entt.hpp>
#include "Component.h"
#include "UID.h"
#include "Ref.h"

namespace Hazard::ECS {

	class Entity;

	class World : public RefCount 
	{
		friend class Entity;
	public:
		World(const std::string& file);
		World(World& other);
		~World();

		void SetName(const std::string& name) { m_Name = name; }

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UID id, const char* name);
		Entity CreateEntity(Entity other);
		Entity GetEntity(entt::entity entity);
		void DestroyEntity(const Entity& entity);

		entt::registry& GetWorldRegistry() { return m_Registry; }
		std::string& GetName() { return m_Name; }
		std::string& GetWorldFile() { return m_File; }
		void SetWorldFile(const std::string& file) { m_File = file; }

		std::tuple<CameraComponent*, TransformComponent*> GetWorldCamera();

		template<typename... T>
		auto GetEntitiesWith() 
		{
			return m_Registry.view<T...>();
		}
	private:
		UID m_Uuid;
		entt::registry m_Registry = entt::registry();
		std::string m_Name = "";
		std::string m_File = "";

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity& entity, T& component);

	};
}
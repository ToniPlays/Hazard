#pragma once

#include <entt.hpp>
#include "Component.h"
#include "Hazard/Core/UID.h"

namespace Hazard::ECS {

	struct WorldData 
	{
		Color clearColor;
	};

	class Entity;

	class World {
		friend class Entity;

	public:
		World(std::string file);
		World(World& other);
		~World();

		entt::registry& GetWorldRegistry() { return m_Registry; }
		std::string& GetName() { return m_Name; }
		std::string& GetWorldFile() { return m_File; }
		WorldData& GetWorldData() {	return m_WorldData;	}

		void RenderAll();

		Entity CreateEntity(const char* name);
		Entity CreateEntity(UID id, const char* name);
		Entity GetEntity(entt::entity id);

		void DestroyEntity(Entity entity);
		void SetName(std::string name) { this->m_Name = name.c_str(); }

		std::tuple<bool, CameraComponent*, TransformComponent*> GetWorldCamera();

		template<typename T>
		std::vector<std::tuple<uint32_t, T>> FindEntitiesWith() 
		{
			std::vector<std::tuple<uint32_t, T>> result;
			auto view = m_Registry.view<T>();

			for (auto entity : view) {
				result.push_back(std::tuple((uint32_t)entity, view.get<T>(entity)));
			}
			return result;
		}
	private:
		UID uid;
		entt::registry m_Registry;
		std::string m_Name;
		std::string m_File;
		WorldData m_WorldData;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity& entity, T& component);

	};
}
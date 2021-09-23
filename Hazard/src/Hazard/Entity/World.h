#pragma once

#include <entt.hpp>
#include "Component.h"
#include "Hazard/Core/UUID.h"

namespace Hazard::ECS {

	enum WorldBackground { Colored = 0, Sky, HDRI, None };

	struct WorldData 
	{
		WorldBackground background = WorldBackground::None;
		//Rendering::BackgroundRenderer* renderer = new Rendering::ColorBackgroundRenderer();
	};

	class Entity;

	class World : public RefCount {
		friend class Entity;

	public:
		World(std::string file);
		World(World& other);
		~World();

		void SetName(std::string name) { m_Name = std::move(name); }
		void SetBackground(WorldBackground type, const std::string& file);

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UUID id, const char* name);
		Entity CreateEntity(Entity other);
		Entity GetEntity(entt::entity entity);
		void DestroyEntity(const Entity& entity);

		entt::registry& GetWorldRegistry() { return m_Registry; }
		std::string& GetName() { return m_Name; }
		std::string& GetWorldFile() { return m_File; }
		WorldData& GetWorldData() {	return m_WorldData;	}
		std::tuple<CameraComponent*, TransformComponent*> GetWorldCamera();

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
		UUID uid;
		entt::registry m_Registry = entt::registry();
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
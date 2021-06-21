#pragma once

#include <entt.hpp>
#include "Component.h"
#include "Hazard/Core/UID.h"
#include "Hazard/Rendering/Sky/BackgroundRenderer.h"

namespace Hazard::ECS {

	enum WorldBackground { Colored = 0, Sky, HDRI, None };

	struct WorldData 
	{
		WorldBackground background = WorldBackground::None;
		Rendering::BackgroundRenderer* renderer = new Rendering::ColorBackgroundRenderer();
	};

	class Entity;

	class World : public RefCount {
		friend class Entity;

	public:
		World(std::string file);
		World(World& other);
		~World();

		entt::registry& GetWorldRegistry() { return m_Registry; }
		std::string& GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = std::move(name); }

		std::string& GetWorldFile() { return m_File; }
		WorldData& GetWorldData() {	return m_WorldData;	}
		void SetBackground(WorldBackground type, const std::string& file);

		void RenderAll();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UID id, const char* name);
		Entity CreateEntity(Entity other);
		Entity GetEntity(entt::entity entity);

		void DestroyEntity(const Entity& entity);
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
		UID uid;
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
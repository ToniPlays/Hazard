#pragma once

#include <entt.hpp>
#include "Component.h"
#include "UID.h"
#include "Ref.h"
#include "Hazard/Assets/Asset.h"
#include "Jobs.h"

namespace Hazard
{
	class Entity;

	class World : public Asset
	{
		friend class Entity;
		friend class WorldAssetLoader;
		friend class WorldDeserializer;

	public:
		World(const std::string& debugName);
		World(World& other) = delete;
		~World();

		AssetType GetType() const override { return AssetType::World; }

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UID id, const std::string& name);
		Entity CreateEntity(Entity other);

		Entity GetEntity(entt::entity entity);
		Entity GetEntityFromUID(const UID& id);

		Entity TryGetEntityFromUID(const UID& id);
		void DestroyEntity(Entity& entity);

		entt::registry& GetWorldRegistry() { return m_Registry; }

		template<typename... T>
		auto GetEntitiesWith()
		{
			return m_Registry.view<T...>();
		}

	public:
		static Ref<World> Copy(Ref<World> sourceWorld);

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity& entity, T& component);

	private:

		std::string m_DebugName;
		entt::registry m_Registry;
		std::unordered_map<UID, Entity> m_EntityUIDMap;
	};
}
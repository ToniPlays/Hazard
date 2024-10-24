#pragma once

#include <entt.hpp>
#include "Component.h"
#include "UID.h"
#include "Ref.h"

#include "Jobs.h"

namespace Hazard
{
	struct WorldAsyncAssetPromise
	{
		AssetType Type;
		JobPromise Promise;
	};
	struct WorldAsyncPromises
	{
		JobPromise WorldPromise;
		std::vector<WorldAsyncAssetPromise> AssetPromises;
	};

	class Entity;

	class World : public Asset
	{
		friend class Entity;
		friend class WorldAssetLoader;

	public:
		World(const std::filesystem::path& file);
		World(World& other) = delete;
		~World();

		void SetName(const std::string& name) { m_Name = name; }

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UID id, const std::string& name);
		Entity CreateEntity(Entity other);
		Entity GetEntity(entt::entity entity);
		Entity GetEntityFromUID(const UID& id);
		Entity TryGetEntityFromUUID(const UID& id);
		void DestroyEntity(const Entity& entity);

		entt::registry& GetWorldRegistry() { return m_Registry; }
		std::string& GetName() { return m_Name; }
		std::filesystem::path& GetWorldFile() { return m_File; }
		void SetWorldFile(const std::filesystem::path& file) { m_File = file; }

		std::vector<WorldAsyncAssetPromise> GetPromises() const { return m_Promises; }

		std::tuple<CameraComponent*, TransformComponent*> GetWorldCamera();

		template<typename... T>
		auto GetEntitiesWith()
		{
			return m_Registry.view<T...>();
		}
	public:
		static Ref<World> Copy(Ref<World> sourceWorld);

	private:
		UID m_Uuid;
		entt::registry m_Registry;
		std::string m_Name = "";
		std::filesystem::path m_File = "";
		std::unordered_map<UID, Entity> m_EntityUIDMap;

		//TODO: Remove
		std::vector<WorldAsyncAssetPromise> m_Promises;

	private:
		template<typename T>
		void OnComponentAdded(Entity& entity, T& component);
		template<typename T>
		void OnComponentRemoved(Entity& entity, T& component);

	};
}
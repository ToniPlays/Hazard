
#include <hzrpch.h>
#include "World.h"
#include "Entity.h"

#define REGISTER_COMPONENT(x)	template<>															\
								void World::OnComponentAdded<x>(Entity& entity, x& component) {}	\
								template<>															\
								void World::OnComponentRemoved<x>(Entity& entity, x& component) {}	\

namespace Hazard 
{
	template<typename T>
	static void CopyComponent(entt::registry& src, entt::registry& dest, const std::unordered_map<UID, entt::entity>& entityMap)
	{
		auto components = src.view<T>();
		for (auto srcEntity : components)
		{
			entt::entity destEntity = entityMap.at(src.get<TagComponent>(srcEntity).Uid);
			auto& srcComponent = src.get<T>(srcEntity);
			auto& destComponent = dest.emplace_or_replace<T>(destEntity, srcComponent);
		}
	}

	World::World(const std::string& file) : m_File(file) 
	{
	
	}

	World::World(World& world)
	{
		m_File = std::string(world.GetWorldFile() + " (copy)");
		m_Name = world.GetName() + " (copy)";
		std::unordered_map<UID, entt::entity> entityMap;

		const auto& entityID = world.m_Registry.view<TagComponent>();

		entityMap.reserve(entityID.size());
		for (size_t i = entityID.size(); i > 0; i--) {
			auto entity = entityID[i - 1];

			TagComponent& c = world.m_Registry.get<TagComponent>(entity);
			UID uuid = c.Uid;

			Entity e = CreateEntity(uuid, c.Tag.c_str());
			entityMap[uuid] = e.GetHandle();
		}

		CopyComponent<TagComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<TransformComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<CameraComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<ScriptComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<SkyLightComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<DirectionalLightComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<PointLightComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<MeshComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<SpriteRendererComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<Rigidbody2DComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<BoxCollider2DComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<CircleCollider2DComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<BatchComponent>(world.m_Registry, m_Registry, entityMap);
	}

	World::~World() 
	{
		std::cout << "Unloaded world: " << m_Name << std::endl;
	}

	Entity World::CreateEntity(const std::string& name)
	{
		Entity entity { m_Registry.create(), this };
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.Uid = {};
		tag.Tag = name;
		entity.AddComponent<TransformComponent>();
		return entity;
	}
	Entity World::CreateEntity(UID id, const char* name)
	{
		Entity e = { m_Registry.create(), this };
		TagComponent& tag = e.AddComponent<TagComponent>();
		tag.Uid = id;
		tag.Tag = name;
		e.AddComponent<TransformComponent>();
		return e;
	}

	template<typename T>
	static void CopyComponentIfExists(entt::entity dst, entt::entity src, entt::registry& registry)
	{
		if (registry.has<T>(src))
		{
			auto& srcComponent = registry.get<T>(src);
			registry.emplace_or_replace<T>(dst, srcComponent);
		}
	}

	Entity World::CreateEntity(Entity other)
	{
		Entity entity;
		if (other.HasComponent<TagComponent>()) {
			std::string tag = other.GetComponent<TagComponent>().Tag;
			entity = CreateEntity(tag);
		}
		else entity = CreateEntity("Duplicate");

		CopyComponentIfExists<TransformComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<CameraComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		CopyComponentIfExists<ScriptComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		CopyComponentIfExists<SkyLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<DirectionalLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<PointLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);


		CopyComponentIfExists<MeshComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<SpriteRendererComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		CopyComponentIfExists<Rigidbody2DComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<BoxCollider2DComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		CopyComponentIfExists<BatchComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		return entity;
	}

	Entity World::GetEntity(entt::entity entity)
	{
		return { entity, this };
	}

	void World::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity);
	}
	std::tuple<CameraComponent*, TransformComponent*> World::GetWorldCamera() {

		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);

		for (auto entity : group) {

			Entity e = GetEntity(entity);
			if (!e.IsVisible()) continue;

			const auto& [cam, transform] = group.get<CameraComponent, TransformComponent>(entity);
			return std::tuple(&cam, &e.GetTransform());
		}
		return std::tuple(nullptr, nullptr);
	}

	//Scene component added and removed
	template<typename T>
	void World::OnComponentAdded(Entity& entity, T& component) {}
	template<typename T>
	void World::OnComponentRemoved(Entity& entity, T& component) {}

	REGISTER_COMPONENT(BatchComponent);
	REGISTER_COMPONENT(SpriteRendererComponent);
	REGISTER_COMPONENT(SkyLightComponent);
	REGISTER_COMPONENT(DirectionalLightComponent);
	REGISTER_COMPONENT(PointLightComponent);
	REGISTER_COMPONENT(Rigidbody2DComponent);
	REGISTER_COMPONENT(BoxCollider2DComponent);
	REGISTER_COMPONENT(CircleCollider2DComponent);
	REGISTER_COMPONENT(ScriptComponent);

	template<>
	void World::OnComponentAdded(Entity& entity, CameraComponent& component) {
		component.RecalculateProjection(1920, 1080);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, CameraComponent& component) {}

	template<>
	void World::OnComponentAdded(Entity& entity, MeshComponent& component) {
		//component.Asset = Rendering::MeshFactory::LoadCube();
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, MeshComponent& component)
	{
	}
}

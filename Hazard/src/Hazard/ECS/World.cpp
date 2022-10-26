
#include <hzrpch.h>
#include "World.h"
#include "Entity.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Scripting/ScriptEngine.h"

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

	World::World(const std::filesystem::path& file) : m_File(file)
	{
		
	}

	World::~World() 
	{
		m_Registry.clear();
	}

	Entity World::CreateEntity(const std::string& name)
	{
		Entity e { m_Registry.create(), this };
		TagComponent& tag = e.AddComponent<TagComponent>();
		tag.Uid = {};
		tag.Tag = name;
		e.AddComponent<TransformComponent>();
		m_EntityUIDMap[tag.Uid] = e;
		return e;
	}
	Entity World::CreateEntity(UID id, const std::string& name)
	{
		Entity e = { m_Registry.create(), this };
		TagComponent& tag = e.AddComponent<TagComponent>();
		tag.Uid = id;
		tag.Tag = name;
		e.AddComponent<TransformComponent>();
		m_EntityUIDMap[tag.Uid] = e;
		return e;
	}

	template<typename T>
	static void CopyComponentIfExists(entt::entity dst, entt::entity src, entt::registry& destRegistry, entt::registry& sourceRegistry)
	{
		if (sourceRegistry.has<T>(src))
		{
			auto& srcComponent = sourceRegistry.get<T>(src);
			destRegistry.emplace_or_replace<T>(dst, srcComponent);
		}
	}

	Entity World::CreateEntity(Entity other)
	{
		Entity entity = { m_Registry.create(), this };
		entt::registry& sourceRegistry = other.GetWorld().GetWorldRegistry();

		CopyComponentIfExists<TagComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<TransformComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<CameraComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);

		CopyComponentIfExists<ScriptComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);

		CopyComponentIfExists<SkyLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<DirectionalLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<PointLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);

		CopyComponentIfExists<MeshComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<SpriteRendererComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);

		CopyComponentIfExists<Rigidbody2DComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
		CopyComponentIfExists<BoxCollider2DComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);

		return entity;
	}

	Entity World::GetEntity(entt::entity entity)
	{
		return { entity, this };
	}

	Entity World::GetEntityFromUID(const UID& id)
	{
		HZR_ASSERT(m_EntityUIDMap.find(id) != m_EntityUIDMap.end(), "UID does not exist");
		return m_EntityUIDMap[id];
	}
	Entity World::TryGetEntityFromUUID(const UID& id)
	{
		if (m_EntityUIDMap.find(id) == m_EntityUIDMap.end()) 
			return Entity();
		return m_EntityUIDMap[id];
	}

	void World::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity);
	}
	std::tuple<CameraComponent*, TransformComponent*> World::GetWorldCamera() {

		auto& group = GetEntitiesWith<CameraComponent, TransformComponent>();

		for (auto entity : group) {

			Entity e = GetEntity(entity);
			if (!e.IsVisible()) continue;

			const auto& [cam, transform] = group.get<CameraComponent, TransformComponent>(entity);
			return std::tuple(&cam, &e.GetTransform());
		}
		return std::tuple(nullptr, nullptr);
	}

	Ref<World> World::Copy(Ref<World> sourceWorld)
	{
		HZR_PROFILE_FUNCTION();
		Ref<World> copied = Ref<World>::Create(sourceWorld->GetWorldFile());
		copied->m_Name = sourceWorld->GetName() + "(copied)";
		const auto& sourceEntities = sourceWorld->GetEntitiesWith<TagComponent>();
		
		copied->m_EntityUIDMap.reserve(sourceEntities.size());

		for (size_t i = sourceEntities.size(); i > 0; i--) 
		{
			const auto& entityID = sourceEntities[i - 1];
			Entity sourceEntity = { entityID, sourceWorld.Raw() };
			const TagComponent& tc = sourceEntity.GetComponent<TagComponent>();

			Entity destEntity = copied->CreateEntity(sourceEntity);
			copied->m_EntityUIDMap[tc.Uid] = destEntity;
		}

		return copied;
	}

	//Scene component added and removed
	template<typename T>
	void World::OnComponentAdded(Entity& entity, T& component) {}
	template<typename T>
	void World::OnComponentRemoved(Entity& entity, T& component) {}

	REGISTER_COMPONENT(MeshComponent);
	REGISTER_COMPONENT(SpriteRendererComponent);
	REGISTER_COMPONENT(SkyLightComponent);
	REGISTER_COMPONENT(DirectionalLightComponent);
	REGISTER_COMPONENT(PointLightComponent);
	REGISTER_COMPONENT(Rigidbody2DComponent);
	REGISTER_COMPONENT(BoxCollider2DComponent);
	REGISTER_COMPONENT(CircleCollider2DComponent);
	REGISTER_COMPONENT(RigidbodyComponent);
	REGISTER_COMPONENT(BoxColliderComponent);
	REGISTER_COMPONENT(SphereColliderComponent);
	
	template<>
	void World::OnComponentAdded(Entity& entity, ScriptComponent& component) {
		Application::GetModule<ScriptEngine>().InitializeComponent(entity, component);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, ScriptComponent& component) {}

	template<>
	void World::OnComponentAdded(Entity& entity, CameraComponent& component) {
		component.RecalculateProjection(1920, 1080);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, CameraComponent& component) {}
}

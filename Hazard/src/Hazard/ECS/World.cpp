
#include <hzrpch.h>
#include "World.h"
#include "Entity.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Scripting/ScriptEngine.h"

#define REGISTER_COMPONENT_FUNC(CompType)	m_Registry.on_construct<CompType>().connect<&World::On##CompType##Added>(this);	\
									m_Registry.on_destroy<CompType>().connect<&World::On##CompType##Removed>(this);

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

	World::World(const std::string& debugName)
	{
		m_DebugName = debugName;
		m_Registry = entt::registry();

		REGISTER_COMPONENT_FUNC(RelationshipComponent);
		REGISTER_COMPONENT_FUNC(ScriptComponent);
		REGISTER_COMPONENT_FUNC(CameraComponent);
	}

	World::~World()
	{
		m_Registry.clear();
	}

	Entity World::CreateEntity(const std::string& name)
	{
		Entity e = { m_Registry.create(), this };
		TagComponent& tag = e.AddComponent<TagComponent>();
		tag.Uid = {};
		tag.Tag = name;

		e.AddComponent<RelationshipComponent>();
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

		e.AddComponent<RelationshipComponent>();
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
		UID id = UID();
		entity.GetTag().Uid = id;
		m_EntityUIDMap[id] = entity;

		CopyComponentIfExists<RelationshipComponent>(entity.GetHandle(), other.GetHandle(), m_Registry, sourceRegistry);
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

	Entity World::TryGetEntityFromUID(const UID& id)
	{
		if (m_EntityUIDMap.find(id) == m_EntityUIDMap.end())
			return Entity();
		return m_EntityUIDMap[id];
	}

	void World::DestroyEntity(Entity& entity)
	{
		//TODO: Call some functions before destroy

		if (entity.HasComponent<RelationshipComponent>())
			entity.RemoveComponent<RelationshipComponent>();

		m_Registry.destroy(entity);
	}

	Ref<World> World::Copy(Ref<World> sourceWorld)
	{
		HZR_PROFILE_FUNCTION();
		Ref<World> copied = Ref<World>::Create(sourceWorld->m_DebugName + " (copy)");
		const auto& sourceEntities = sourceWorld->GetEntitiesWith<TagComponent>();

		copied->m_EntityUIDMap.reserve(sourceEntities.size());

		for (uint64_t i = sourceEntities.size(); i > 0; i--)
		{
			const auto& entityID = sourceEntities[i - 1];
			Entity sourceEntity = { entityID, sourceWorld.Raw() };
			const TagComponent& tc = sourceEntity.GetComponent<TagComponent>();

			Entity destEntity = copied->CreateEntity(sourceEntity);
			copied->m_EntityUIDMap[destEntity.GetUID()] = destEntity;
		}

		return copied;
	}

	//Scene component added and removed
	void World::OnRelationshipComponentAdded(entt::registry& registry, entt::entity entity) 
	{
		Entity e = { entity, this };
		auto& rsc = e.GetComponent<RelationshipComponent>();
		Entity parent = TryGetEntityFromUID(rsc.ParentHandle);
		if (!parent) return;

		parent.GetComponent<RelationshipComponent>().ChildHandles.push_back(e.GetUID());
	}
	void World::OnRelationshipComponentRemoved(entt::registry& registry, entt::entity entity)
	{
		Entity e = { entity, this };
		auto& component = e.GetComponent<RelationshipComponent>();

		Entity parent = TryGetEntityFromUID(component.ParentHandle);
		if (!parent) return;

		HZR_CORE_INFO("Removing child {} of {}", e.GetTag().Tag, parent.GetTag().Tag);
		parent.RemoveChild(e);
	}

	void World::OnCameraComponentAdded(entt::registry& registry, entt::entity entity)
	{
		Entity e = { entity, this };
		auto& component = e.GetComponent<CameraComponent>();
		component.RecalculateProjection(1920, 1080);
	}
	void World::OnCameraComponentRemoved(entt::registry& registry, entt::entity entity) {}

	void World::OnScriptComponentAdded(entt::registry& registry, entt::entity entity) {}
	void World::OnScriptComponentRemoved(entt::registry& registry, entt::entity entity) {}
}

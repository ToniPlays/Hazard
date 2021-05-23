#pragma once

#include <hzrpch.h>
#include "World.h"
#include "Entity.h"
#include "SceneCommand.h"
#include "Hazard/Audio/AudioCommand.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

namespace Hazard::ECS {


	template<typename T>
	static void CopyComponent(entt::registry& src, entt::registry& dest, const std::unordered_map<UID, entt::entity>& entityMap)
	{
		auto components = src.view<T>();
		for (auto srcEntity : components)
		{
			entt::entity destEntity = entityMap.at(src.get<TagComponent>(srcEntity).m_ID);
			auto& srcComponent = src.get<T>(srcEntity);
			auto& destComponent = dest.emplace_or_replace<T>(destEntity, srcComponent);
		}
	}

	World::World(std::string file) : m_File(file) {}

	World::World(World& world)
	{
		m_File = std::string(world.GetWorldFile() + " (copy)").c_str();
		m_Name = world.GetName() + " (copy)";
		std::unordered_map<UID, entt::entity> entityMap;

		memcpy(&m_WorldData, &world.m_WorldData, sizeof(WorldData));
		auto& entityID = world.m_Registry.view<TagComponent>();

		entityMap.reserve(entityID.size());

		for (size_t i = entityID.size(); i > 0; i--) {
			auto entity = entityID[i - 1];

			TagComponent& c = world.m_Registry.get<TagComponent>(entity);
			UID uid = c.m_ID;

			Entity e = CreateEntity(uid, c.m_Tag.c_str());
			entityMap[uid] = e.GetHandle();
		}

		CopyComponent<TagComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<TransformComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<ScriptComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<VisualScriptComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<CameraComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<AudioSourceComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<MeshComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<SpriteRendererComponent>(world.m_Registry, m_Registry, entityMap);

		CopyComponent<SkyLightComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<DirectionalLightComponent>(world.m_Registry, m_Registry, entityMap);
		CopyComponent<PointLightComponent>(world.m_Registry, m_Registry, entityMap);
	}

	World::~World() {}

	void World::SetBackground(WorldBackground type, std::string file)
	{
		if (type != m_WorldData.background) {
			if (m_WorldData.renderer) delete m_WorldData.renderer;

			if (type == WorldBackground::Colored) {
				m_WorldData.renderer = new Rendering::ColorBackgroundRenderer();
			}
			else {
				m_WorldData.renderer = new Rendering::SkyboxBackgroundRenderer(file);
			}
		}
		m_WorldData.background = type;
	}

	void World::RenderAll() {

		HZR_PROFILE_FUNCTION();
		auto sprites = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		auto meshes = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);

		for (auto entity : meshes) {

			Entity e = GetEntity(entity);
			auto& [mesh, transform] = m_Registry.get<MeshComponent, TransformComponent>(entity);
			if (!e.IsVisible() || !mesh.m_Mesh) continue;

			SceneCommand::Render(mesh, transform);
		}
		for (auto entity : sprites) {

			Entity e{ entity, this };
			if (!e.IsVisible()) continue;

			auto& [sprite, transform] = m_Registry.get<SpriteRendererComponent, TransformComponent>(entity);
			if (!sprite.m_Texture->GetData().hasTransparency)
				SceneCommand::Render(sprite, transform);
		}

		for (auto entity : sprites) {

			Entity e{ entity, this };
			if (!e.IsVisible()) continue;

			auto& [sprite, transform] = m_Registry.get<SpriteRendererComponent, TransformComponent>(entity);

			if (sprite.m_Texture->GetData().hasTransparency)
				SceneCommand::Render(sprite, transform);
		}

		auto batches = m_Registry.group<BatchComponent>(entt::get<TransformComponent>);
		for (auto entity : batches) {

			Entity e = GetEntity(entity);
			if (!e.IsVisible()) continue;

			auto& [batch, transform] = m_Registry.get<BatchComponent, TransformComponent>(entity);
			SceneCommand::Render(batch, transform);
		}
	}
	Entity World::CreateEntity(const std::string& name)
	{
		Entity entity{ m_Registry.create(), this };
		TagComponent& tag = entity.AddComponent<TagComponent>();
		tag.m_Tag = name;
		tag.m_ID = {};
		entity.AddComponent<TransformComponent>();
		return entity;
	}
	Entity World::CreateEntity(UID id, const char* name)
	{
		Entity e = { m_Registry.create(), this };
		TagComponent& tag = e.AddComponent<TagComponent>();
		tag.m_ID = id;
		tag.m_Tag = name;
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
			std::string tag = other.GetComponent<TagComponent>().m_Tag;
			entity = CreateEntity(tag);
		}
		else entity = CreateEntity("Duplicate");

		CopyComponentIfExists<TransformComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<CameraComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<ScriptComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<VisualScriptComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<MeshComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<AudioSourceComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<SpriteRendererComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);

		CopyComponentIfExists<SkyLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<DirectionalLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);
		CopyComponentIfExists<PointLightComponent>(entity.GetHandle(), other.GetHandle(), m_Registry);


		return entity;
	}

	Entity World::GetEntity(entt::entity id)
	{
		Entity e{ id, this };
		return e;
	}
	void World::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
	std::tuple<bool, CameraComponent*, TransformComponent*> World::GetWorldCamera() {

		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);

		for (auto entity : group) {

			Entity e = GetEntity(entity);
			if (!e.IsVisible()) continue;

			auto& [cam, transform] = group.get<CameraComponent, TransformComponent>(entity);
			return std::tuple(true, &cam, &e.GetTransform());
		}
		return std::tuple(false, nullptr, nullptr);
	}

#pragma region Component added and removed methods
	//Scene component added and removed
	template<typename T>
	void World::OnComponentAdded(Entity& entity, T& component) {}
	template<typename T>
	void World::OnComponentRemoved(Entity& entity, T& component) {}
	//CAMERA COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, CameraComponent& component) {
		component.RecalculateProjection(1920, 1080);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, CameraComponent& component) {}

	//SPRITE RENDERER COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, SpriteRendererComponent& component) {
		component.m_Texture = Rendering::RenderUtils::Get<Rendering::Texture2D>();
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, SpriteRendererComponent& component) {}
	//BATCH COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, BatchComponent& component) {}
	template<>
	void World::OnComponentRemoved(Entity& entity, BatchComponent& component) {}
	template<>
	void World::OnComponentAdded(Entity& entity, AudioSourceComponent& component) 
	{
		component.source = Audio::AudioCommand::Create(component);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, AudioSourceComponent& component) {}
	//SKY LIGHT COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, SkyLightComponent& component) {}
	template<>
	void World::OnComponentRemoved(Entity& entity, SkyLightComponent& component) {}
	//DIRECTIONAL LIGHT COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, DirectionalLightComponent& component) {}
	template<>
	void World::OnComponentRemoved(Entity& entity, DirectionalLightComponent& component) {}
	//POINT LIGHT COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, PointLightComponent& component) {}
	template<>
	void World::OnComponentRemoved(Entity& entity, PointLightComponent& component) {}
	//SCRIPT COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, ScriptComponent& component) {
		SceneCommand::OnScriptAttached(entity, component);
	}
	template<>
	void World::OnComponentAdded(Entity& entity, VisualScriptComponent& component) {
		SceneCommand::OnScriptAttached(entity, component);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, ScriptComponent& component)
	{
		SceneCommand::OnScriptDetached(entity, component);
	}
	template<>
	void World::OnComponentRemoved(Entity& entity, VisualScriptComponent& component)
	{
		SceneCommand::OnScriptDetached(entity, component);
	}
	//MESH COMPONENT
	template<>
	void World::OnComponentAdded(Entity& entity, MeshComponent& component) {}
	template<>
	void World::OnComponentRemoved(Entity& entity, MeshComponent& component) {}
#pragma endregion
}
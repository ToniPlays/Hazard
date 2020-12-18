#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Components.h"

#include "Entity.h"

namespace Hazard {

	namespace ECS {

		Scene::Scene()
		{
			file = "Empty scene";
			CreateEntity();
			CreateEntity("Dumb entity");
		}

		Scene::Scene(const char* _file) : file(file)
		{
			
		}

		Scene::~Scene()
		{

		}

		Entity Scene::CreateEntity(const std::string& name)
		{
			Entity entity = { registry.create(), this };
			entity.AddComponent<TransformComponent>();
			TagComponent& tag = entity.AddComponent<TagComponent>();
			tag.Tag = name;
			HZR_CORE_INFO("Entity " + name + " created");
			return entity;
		}

		void Scene::DestroyEntity(Entity entity)
		{
			registry.destroy(entity);
		}

		template<typename T>
		void Scene::OnComponentAdded(Entity entity, T& component) {
			static_assert(false);
		}
		template<>
		void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<MeshRendererComponent>(Entity entity, MeshRendererComponent& component) {

		}
		template<>
		void Scene::OnComponentAdded<SpriteRenderer>(Entity entity, SpriteRenderer& component) {

		}
	}
}
#pragma once

#include <hzrpch.h>
#include "World.h"
#include "Entity.h"
#include "SceneCommand.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

namespace Hazard::ECS {

    World::World(std::string file) : file(file)
    {
        
    }
    World::~World()
    {

    }
        
    void World::Flush()
    {
        
    }
    void World::RenderAll() {

        auto sprites = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : sprites) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto& [sprite, transform] = registry.get<SpriteRendererComponent, TransformComponent>(entity);
            SceneCommand::Render(sprite, transform);
        }
        auto batches = registry.group<BatchComponent>(entt::get<TransformComponent>);
        for (auto entity : batches) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto& [batch, transform] = registry.get<BatchComponent, TransformComponent>(entity);
            SceneCommand::Render(batch, transform);
        }
        auto meshes = registry.group<MeshComponent>(entt::get<TransformComponent>);
        for (auto entity : meshes) {

            Entity e{ entity, this };
            auto& [mesh, transform] = registry.get<MeshComponent, TransformComponent>(entity);
            if (!e.IsVisible() || mesh.mesh == nullptr) continue;

            SceneCommand::Render(mesh, transform);
        }
    }
    Entity World::CreateEntity(const char* name)
    {
        Entity entity{ registry.create(), this };
        entity.AddComponent<TagComponent>().tag = name;
        entity.AddComponent<TransformComponent>();
        return entity;
    }
    Entity World::GetEntity(entt::entity id)
    {
        Entity e{ id, this };
        return e;
    }
    void World::DestroyEntity(Entity entity)
    {
        registry.destroy(entity);
    }
    std::tuple<bool, CameraComponent*, TransformComponent*> World::GetWorldCamera() {

        auto group = registry.group<CameraComponent>(entt::get<TransformComponent>);

        for (auto entity : group) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto&[cam, transform] = group.get<CameraComponent, TransformComponent>(entity);
            return std::tuple(true, &cam, &e.GetTransform());
        }
        return std::tuple(false, nullptr, nullptr);
    }

#pragma region Component added and removed methods
    //Scene component added and removed
    template<typename T>
    void World::OnComponentAdded(Entity& entity, T& component) {
        
    }
    template<typename T>
    void World::OnComponentRemoved(Entity& entity, T& component)
    {

    }
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
        component.texture = Rendering::RenderUtils::GetFromTextures(0);
    }
    template<>
    void World::OnComponentRemoved(Entity& entity, SpriteRendererComponent& component) {}
    //BATCH COMPONENT
    template<>
    void World::OnComponentAdded(Entity& entity, BatchComponent& component) {}
    template<>
    void World::OnComponentRemoved(Entity& entity, BatchComponent& component) {}
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
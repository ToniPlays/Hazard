#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Entity.h"
#include "Loader/SceneCommand.h"

namespace Hazard::ECS {

    Scene::Scene(std::string file) : file(file)
    {
        
    }
    Scene::~Scene()
    {

    }
        
    void Scene::Flush()
    {
        
    }
    void Scene::RenderAll() {

        auto sprites = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : sprites) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto& [sprite, transform] = registry.get<SpriteRendererComponent, TransformComponent>(entity);
            SceneCommand::Render(sprite, transform);
        }
        auto group = registry.group<BatchComponent>(entt::get<TransformComponent>);
        for (auto entity : group) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto& [batch, transform] = registry.get<BatchComponent, TransformComponent>(entity);
            SceneCommand::Render(batch, transform);
        }
    }
    Entity Scene::CreateEntity(const char* name)
    {
        Entity entity{ registry.create(), this };
        entity.AddComponent<TagComponent>().tag = name;
        entity.AddComponent<TransformComponent>();
        return entity;
    }
    void Scene::DestroyEntity(Entity entity)
    {
        registry.destroy(entity);
    }
    std::tuple<bool, CameraComponent, TransformComponent> Scene::GetSceneCamera() {

        auto group = registry.group<CameraComponent>(entt::get<TransformComponent>);

        for (auto entity : group) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto&[cam, transform] = group.get<CameraComponent, TransformComponent>(entity);
            return std::tuple(true, cam, e.GetTransform());
        }
        return std::tuple(false, CameraComponent(), TransformComponent());
    }
#pragma region Component added and removed methods
    //Scene component added and removed
    template<typename T>
    void Scene::OnComponentAdded(Entity& entity, T& component) {
        
    }
    template<typename T>
    void Scene::OnComponentRemoved(Entity& entity, T& component)
    {

    }
    //CAMERA COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, CameraComponent& component) {
        component.RecalculateProjection(1920, 1080);
    }
    template<>
    void Scene::OnComponentRemoved(Entity& entity, CameraComponent& component) {}

    //SPRITE RENDERER COMPONENT

    template<>
    void Scene::OnComponentAdded(Entity& entity, SpriteRendererComponent& component) {
        component.texture = Rendering::RenderUtils::GetFromTextures(0);
    }
    template<>
    void Scene::OnComponentRemoved(Entity& entity, SpriteRendererComponent& component) {}
    //BATCH COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, BatchComponent& component) {}
    template<>
    void Scene::OnComponentRemoved(Entity& entity, BatchComponent& component) {}
    //SKY LIGHT COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, SkyLightComponent& component) {}
    template<>
    void Scene::OnComponentRemoved(Entity& entity, SkyLightComponent& component) {}
    //DIRECTIONAL LIGHT COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, DirectionalLightComponent& component) {}
    template<>
    void Scene::OnComponentRemoved(Entity& entity, DirectionalLightComponent& component) {}
    //POINT LIGHT COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, PointLightComponent& component) {}
    template<>
    void Scene::OnComponentRemoved(Entity& entity, PointLightComponent& component) {}
    //SCRIPT COMPONENT
    template<>
    void Scene::OnComponentAdded(Entity& entity, ScriptComponent& component) {
        SceneCommand::OnScriptAttached(entity, component);
    }
    template<>
    void Scene::OnComponentRemoved(Entity& entity, ScriptComponent& component)
    {
        SceneCommand::OnScriptDetached(entity, component);
    }
#pragma endregion
}
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
    void Scene::Render()
    {
        auto sprites = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);

        for (auto entity : sprites) {
            auto& [sprite, transform] = registry.get<SpriteRendererComponent, TransformComponent>(entity);
            SceneCommand::Render(sprite, transform);
        }
        auto group = registry.group<BatchComponent>(entt::get<TransformComponent>);
        for (auto entity : group) {
            auto& [batch, transform] = registry.get<BatchComponent, TransformComponent>(entity);
            SceneCommand::Render(batch, transform);
        }
    }
    void Scene::Flush()
    {
        
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
    std::tuple<bool, Camera> Scene::GetSceneCamera() {

        auto group = registry.group<CameraComponent>(entt::get<TransformComponent>);

        for (auto entity : group) {
            auto&[transform, cam] = group.get<TransformComponent, CameraComponent>(entity);
            return std::tuple(true, Camera(cam, cam.projection * glm::inverse(transform.GetTransformMat4()), transform.Translation));
        }
        return std::tuple(false, Camera());
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity& entity, T& component) {
        
    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, CameraComponent& component) {
        component.RecalculateProjection(1920, 1080);
    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, SpriteRendererComponent& component) {
        component.texture = Rendering::RenderUtils::GetFromTextures(0);
    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, BatchComponent& component) {

    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, SkyLightComponent& component) {

    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, DirectionalLightComponent& component) {

    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, PointLightComponent& component) {

    }
}
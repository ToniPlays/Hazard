#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Entity.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/2D/QuadData.h"

namespace Hazard::ECS {

    Scene::Scene(std::string file) : file(file)
    {
        
    }
    Scene::~Scene()
    {

    }
    void Scene::Render()
    {
        auto group = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
        Rendering::RenderEngine& engine = Core::HazardLoop::GetModule<Rendering::RenderEngine>();

        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            engine.Submit(Rendering::Quad{ transform.GetTransformMat4(), { sprite.tint.r, sprite.tint.g, sprite.tint.b, sprite.tint.a } });
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
            return std::tuple(true, Camera(cam, cam.projection * glm::inverse(transform.GetTransformMat4())));
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

    }
}
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
        Rendering::RenderEngine& engine = Application::GetModule<Rendering::RenderEngine>();
        auto sprites = registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);

        for (auto entity : sprites) {

            Entity e{ entity, this };
            if (!e.IsVisible()) continue;

            auto& [transform, sprite] = sprites.get<TransformComponent, SpriteRendererComponent>(entity);

            engine.Submit(Rendering::Quad{ transform.GetTransformMat4(),
                { sprite.tint.r, sprite.tint.g, sprite.tint.b, sprite.tint.a }, sprite.texture });
        }
        auto group = registry.group<BatchComponent>(entt::get<TransformComponent>);

        for (auto entity : group) {
            Entity e{ entity, this };
            if (!e.IsVisible()) continue;
            auto& [transform, batch] = group.get<TransformComponent, BatchComponent>(entity);

            for (int x = 0; x < batch.size; x++) {
                for (int y = 0; y < batch.size; y++) {

                    glm::vec3 translation = transform.Translation + glm::vec3{ x * 1.1, y * 1.1, 1.0 };

                    engine.Submit(Rendering::Quad { Math::ToTransformMatrix(translation, transform.Rotation, transform.Scale),
                        { batch.tint.r, batch.tint.g, batch.tint.b, batch.tint.a }, nullptr });
                }
            }
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
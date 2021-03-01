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
        auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        Rendering::RenderEngine& engine = Core::HazardLoop::GetModule<Rendering::RenderEngine>();

        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            for (int i = 0; i < 1000; i++) {
                engine.Submit(Rendering::Quad{ transform.GetTransformMat4(), { sprite.tint.r, sprite.tint.g, sprite.tint.b, sprite.tint.a } });
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
    template<typename T>
    void Scene::OnComponentAdded(Entity& entity, T& component) {
        
    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, CameraComponent& component) {

    }
    template<>
    void Scene::OnComponentAdded(Entity& entity, SpriteRendererComponent& component) {

    }
}
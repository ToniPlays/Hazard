#pragma once

#include <hzrpch.h>
#include "Scene.h"
#include "Entity.h"

namespace Hazard::ECS {

    Scene::Scene() : file("EmptyFile")
    {
        Entity entity = CreateEntity("Test entity");
        
        HZR_CORE_INFO("Created entity " + std::string(entity.GetComponent<TagComponent>().tag));
    }
    Scene::Scene(const char* file) : file(file)
    {
        
    }
    Scene::~Scene()
    {

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
}
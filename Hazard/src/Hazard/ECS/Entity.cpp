#pragma once
#include <hzrpch.h>
#include "Entity.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRenderer.h"
#include "Hazard/Modules/Scene/SceneManager.h"

namespace Hazard {

	
	Entity::Entity(std::string _name)
	{
		this->name = _name;
	}
	Entity::Entity(std::string _name, std::initializer_list<Component*> _components)
	{
		this->name = _name;
		for (Component* component : _components) {
			component->SetParent(this);
			components.push_back(component);
		}
	}
	
	Entity::~Entity()
	{
		for (Entity* child : childs) {
			RemoveEntity(child);
			delete child;
		}
		for (Component* component : components) {
			RemoveComponent(component);
			delete component;
		}

	}

	void Entity::Awake()
	{
		for (Component* component : components) {
			component->OnAttach();
		}
	}

	void Entity::AddComponent(Component* component) {
		component->SetParent(this);
		components.push_back(component);
		ModuleHandler::GetModule<SceneManager>()->GetActiveScene()->OnComponentAdded(component);
	}

	void Entity::RemoveComponent(Component* component) {
		auto& it = std::find(components.begin(), components.end(), component);
		if (it != components.end()) {
			components.erase(it);
			component->OnDetach();
			ModuleHandler::GetModule<SceneManager>()->GetActiveScene()->OnComponentRemoved(component);
		}
	}

	void Entity::AddEntity(Entity* entity) {
		childs.push_back(entity);
	}
	void Entity::RemoveEntity(Entity* entity) {
		auto iterator = std::find(childs.begin(), childs.end(), entity);
		if (iterator != childs.end()) {
			entity->OnDestroy();
			childs.erase(iterator);
		}
	}
	void Entity::Render()
	{
	}
	void Entity::OnDestroy() {
		
	}
}
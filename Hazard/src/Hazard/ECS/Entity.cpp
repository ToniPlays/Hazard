#pragma once
#include <hzrpch.h>
#include "Entity.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"

namespace Hazard {

	
	Entity::Entity(std::string _name)
	{
		this->components = components;
		this->name = _name;
		AddComponent<Transform>();
	}
	Entity::Entity(std::string _name, std::initializer_list<Component*> _components)
	{
		this->components = components;
		this->name = _name;
		for (Component* component : _components) {
			this->components.push_back(component);
		}
	}
	
	Entity::~Entity()
	{
		for (Component* component : components) {
			delete component;
		}
	}
	void Entity::RemoveComponent(Component* component) {
		auto& it = std::find(components.begin(), components.end(), component);
		if (it != components.end()) {
			components.erase(it);
		}

	}
	void Entity::Render()
	{
		for (Component* comp : components) {
			
		}
	}
	void Entity::OnDestroy() {
		
	}
}
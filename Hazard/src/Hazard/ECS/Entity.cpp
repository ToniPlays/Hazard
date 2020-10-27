#pragma once
#include <hzrpch.h>
#include "Entity.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"

namespace Hazard {

	
	Entity::Entity(std::string _name)
	{
		this->name = _name;
	}
	Entity::Entity(std::string _name, std::initializer_list<Component*> _components)
	{
		this->name = _name;
		for (Component* component : _components) {
			this->components.push_back(component);
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
	void Entity::RemoveComponent(Component* component) {
		auto& it = std::find(components.begin(), components.end(), component);
		if (it != components.end()) {
			components.erase(it);
		}
	}
	void Entity::AddEntity(Entity* entity) {

		if (entity == nullptr)
			entity = new Entity("Empty entity");

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
		for (Component* comp : components) {
			
		}
	}
	void Entity::OnDestroy() {
		
	}
}
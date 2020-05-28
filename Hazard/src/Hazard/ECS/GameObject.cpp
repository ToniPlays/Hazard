#pragma once
#include <hzrpch.h>
#include "GameObject.h"
#include "Hazard/ECS/Components/MeshRenderer.h"
#include "Hazard/Modules/Rendering/RendererAPI.h"

namespace Hazard {

	
	GameObject::GameObject(std::string _name)
	{
		this->components = components;
		this->name = _name;
	}
	GameObject::GameObject(std::string _name, std::initializer_list<Component*> _components)
	{
		this->components = components;
		this->name = _name;
		transform = Transform();
		Mesh* mesh = new Mesh();
		components.emplace_back(new MeshRenderer(mesh, RendererAPI::Shader("res/shaders/shader.glsl"), this));
	}


	GameObject::~GameObject()
	{
		for (Component* component : components) {
			delete component;
		}
	}

	void GameObject::Render()
	{
		for (Component* comp : components) {
			comp->OnRender();
		}
	}

}
#pragma once
#include <hzrpch.h>
#include "GameObject.h"
#include "Hazard/ECS/Components/MeshRenderer.h"
#include "Hazard/Modules/Rendering/RendererAPI.h"
#include "Hazard/Utils/Loaders/MeshLoader.h"

namespace Hazard {

	
	GameObject::GameObject(std::string _name, Transform transform)
	{
		this->components = components;
		this->name = _name;
		this->transform = transform;
	}
	GameObject::GameObject(std::string _name, std::initializer_list<Component*> _components, Transform transform)
	{
		this->components = components;
		this->name = _name;
		this->transform = transform;
		Mesh* mesh = MeshLoader::LoadMesh("res/models/cube.obj");
		components.emplace_back(new MeshRenderer(mesh, RendererAPI::Shader("res/shaders/default.glsl"), this));
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
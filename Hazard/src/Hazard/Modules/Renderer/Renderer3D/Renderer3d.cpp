#pragma once
#include <hzrpch.h>
#include "Renderer3d.h"
#include "glad/glad.h"
#include "Hazard/ECS/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/glm.hpp"
#include "../Grid.h"


namespace Hazard {
	glm::mat4 Renderer3D::projection;

	std::vector<GameObject*> Renderer3D::gameObjects;

	Renderer3D::Renderer3D()
	{
		std::cout << "Renderer3D init" << std::endl;
		Camera* camera = new Camera();
		camera->SetSize(10);

		//camera->transform.rotation.x = -50;
		camera->transform.position = { 0, 0, 10 };
		GameObject* gameObject = new GameObject("Test object 1", {});

		gameObject->transform.rotation.y = 90.0f;
		gameObject->transform.position.x = 4;
		gameObjects.push_back(gameObject);
		//gameObjects.push_back(Origin);
		gameObjects.push_back(camera);
	}

	Renderer3D::~Renderer3D()
	{

	}

	void Renderer3D::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		if (resizeEvent.GetWidth() != 0 && resizeEvent.GetHeight() != 0)
			projection = glm::perspective((float)Math::ToRadians(60.0f), 
				(float)resizeEvent.GetWidth() / (float)resizeEvent.GetHeight(), 0.1f, 1000.0f);
	}

	void Renderer3D::Render()
	{
		
		PROFILE_FN();
		for (GameObject* gameObject : gameObjects) {
			gameObject->Render();
		}
		PROFILE_FN();
	}
}
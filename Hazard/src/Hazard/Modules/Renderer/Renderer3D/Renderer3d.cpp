#pragma once
#include <hzrpch.h>
#include "Renderer3d.h"
#include "glad/glad.h"
#include "Hazard/ECS/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/glm.hpp"
#include "../Grid.h"


namespace Hazard {

	float Renderer3D::test = 45.0;
	bool Renderer3D::useGradient = true;
	glm::mat4 Renderer3D::projection;

	std::vector<GameObject*> Renderer3D::gameObjects;

	Renderer3D::Renderer3D()
	{
		
		Camera* camera = new Camera();
		camera->SetSize(1);
		camera->transform.rotation.x = -50;
		camera->transform.position = { 0.5, -2, 8 };
		GameObject* gameObject = new GameObject("Test object 1", {});

		gameObject->transform.rotation.y = 90.0f;
		gameObjects.push_back(gameObject);
		//gameObjects.push_back(Origin);
		gameObjects.push_back(camera);

		HZR_CORE_INFO("Renderer 3D enabled");
	}

	Renderer3D::~Renderer3D()
	{

	}

	void Renderer3D::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		float scale = 5;
		float aspectX = (float)resizeEvent.GetWidth() / (float)resizeEvent.GetHeight() * scale;
		float aspectY = (float)resizeEvent.GetWidth() / (float)resizeEvent.GetHeight() * scale;
		//Orthographic
		//projection = glm::ortho(-aspectX, aspectX, -aspectY, aspectY, 0.1f, 1000.0f);
		//Perspective
		projection = glm::perspective((float)Math::ToRadians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	}

	void Renderer3D::Render()
	{

		PROFILE_FN();
		int calls = 0;

		for (GameObject* gameObject : gameObjects) {
			gameObject->Render();
			calls++;
		}

		std::stringstream ss;
		ss << calls;
		HazardLoop::GetCurrent().GetAppInfo().SetValue("DrawCalls", ss.str());
		PROFILE_FN();
	}
}
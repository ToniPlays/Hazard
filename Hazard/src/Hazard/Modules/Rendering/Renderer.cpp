#pragma once
#include <hzrpch.h>
#include "Renderer.h"
#include "glad/glad.h"
#include "RendererAPI.h"
#include "Hazard/ECS/Camera.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Hazard {

	glm::mat4& Renderer::GetProjection()
	{
		return projection;
	}

	float Renderer::test = 45.0;
	bool Renderer::useGradient = true;
	glm::mat4 Renderer::projection;

	std::vector<GameObject*> Renderer::gameObjects;

	Renderer::Renderer() : Module("Renderer")
	{
		HZR_CORE_INFO("Renderer enabled");

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT(Renderer::OnEvent));

		Camera* camera = new Camera();
		camera->SetSize(1);
		camera->transform.rotation.x = 0;
		camera->transform.position = { 0, 0, 5 };
		GameObject* gameObject = new GameObject("Test object 1", {});
		//GameObject* Origin = new GameObject("Origin", {});

		//Origin->transform.scale = Vector3<float>(0.025, 0.025, 0.025);
		gameObject->transform.rotation.y = 25.0f;
		gameObjects.push_back(gameObject);
		//gameObjects.push_back(Origin);
		gameObjects.push_back(camera);

		WindowResizeEvent event(window->GetWidth(), window->GetHeight());
		Resized(event);
	}

	Renderer::~Renderer()
	{
		for (GameObject* gameObject : gameObjects) {
			gameObject->~GameObject();
		}
	}

	bool Renderer::Resized(Event& e)
	{
		float scale = 5;
		float aspectX = (float)window->GetWidth() / (float)window->GetWidth() * scale;
		float aspectY = (float)window->GetHeight() / (float)GetWindow().GetWidth() * scale;
		//Orthographic
		//projection = glm::ortho(-aspectX, aspectX, -aspectY, aspectY, 0.1f, 1000.0f);
		//Perspective
		projection = glm::perspective((float)Math::ToRadians(90.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);
		return true;
	}

	void Renderer::Render()
	{
		window->OnUpdate();

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

	bool Renderer::OnEvent(Event& e)
	{	
		EventDispatcher dispatcher(e);
		if(dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(Renderer::Resized))) return true;
		return HazardLoop::GetCurrent().OnEvent(e);
	}
}
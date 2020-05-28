#pragma once
#include <hzrpch.h>
#include "Renderer.h"
#include "glad/glad.h"
#include "RendererAPI.h"
#include "Hazard/ECS/Camera.h"

namespace Hazard {

	float Renderer::test = 1.0;
	bool Renderer::useGradient = false;
	Matrix4 Renderer::viewMatrix;

	std::vector<GameObject*> Renderer::gameObjects;

	Renderer::Renderer() : Module("Renderer")
	{
		HZR_CORE_INFO("Renderer enabled");

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT(Renderer::OnEvent));

		Camera* camera = new Camera();
		GameObject* gameObject = new GameObject("Test object 1", {});
		GameObject* Origin = new GameObject("Origin", {});

		Origin->transform.scale = Vector3<float>(0.05, 0.05, 0.05);
		gameObjects.push_back(gameObject);
		gameObjects.push_back(camera);
		gameObjects.push_back(Origin);

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

		viewMatrix = Matrix4::Ortho(-aspectX, aspectX, -aspectY, aspectY, -1000, 1000);
		return true;
	}

	void Renderer::Render()
	{
		PROFILE_FN();
		window->OnUpdate();
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
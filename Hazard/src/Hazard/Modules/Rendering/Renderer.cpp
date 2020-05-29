#pragma once
#include <hzrpch.h>
#include "Renderer.h"
#include "glad/glad.h"
#include "RendererAPI.h"
#include "Hazard/ECS/Camera.h"

namespace Hazard {

	Matrix4& Renderer::GetViewMatrix()
	{
		return Matrix4::Mul(viewMatrix, Matrix4::TransformMatrix(Camera::GetTransform()));
	}

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
		camera->SetSize(5);
		camera->transform.position = { 0.2f, -0.2f, 0.0f };
		GameObject* gameObject = new GameObject("Test object 1", {});
		GameObject* Origin = new GameObject("Origin", {});

		Origin->transform.scale = Vector3<float>(0.025, 0.025, 0.025);

		gameObjects.push_back(gameObject);
		gameObjects.push_back(Origin);
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
		//Ortho
		viewMatrix = Matrix4::Ortho(-aspectX, aspectX, -aspectY, aspectY, -1000, 1000);
		//Perspective
		//viewMatrix = Matrix4::Perspec(70.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 1000.0f);
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
#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Hazard {

#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)


	//Static variables
	Application* Application::instance = nullptr;
	ApplicationInfo* Application::info;

	//Application constructor
	Application::Application(std::string name) {

		info = new ApplicationInfo(name);

		instance = this;
		this->layerStack = LayerStack();
		Log::Init();

		window = std::unique_ptr<Window>(Window::Create(WindowProps(info->title)));
		window->SetEventCallback(BIND_EVENT(Application::OnEvent));

	}
	Application::~Application() {
		
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(OnWindowClose));
	}
	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer)
	{
		layerStack.PushOverlay(layer);
	}
	void Application::Run()
	{
		APPStart();

		while (isRunning) {

			//Calulate deltatime
			double time = glfwGetTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;


			//Update and render;
			APPUpdate();
			APPRender();

			lastTime = time;
		}
		APPCleanUp();
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}
	void Application::APPStart() {

		Time::time = 0;
		lastTime = glfwGetTime();
		renderer = new Renderer2D();

		//Set Application info
		info->renderer.renderer = GetWindow().GetContext().Get(SYSTEM_GPU);
		info->renderer.graphicProcessor = GetWindow().GetContext().Get(SYSTEM_RENDERER);

		Start();
	}

	void Application::APPUpdate() {
		window->OnUpdate();
	}

	void Application::APPRender() {

		//Render scene
		renderer->Render();
		//Update layers
		for (Layer* layer : layerStack) {
			layer->OnUpdate();
		}
	}
	void Application::APPCleanUp() {
		
	}
}
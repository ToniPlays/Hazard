#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "ImGui/ImGuiLayer.h"
#include "glad/glad.h"
#include "imgui.h"
#include "Platform/OpenGl/imgui_impl_glfw.h"
#include "Platform/OpenGl/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace Hazard {

#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	std::string Application::name = "Untitled";

	Application* Application::instance = nullptr;

	Application::Application(std::string name) {

		this->name = name;
		instance = this;
		this->layerStack = LayerStack();
		Log::Init();

		window = std::unique_ptr<Window>(Window::Create());
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
		Start();

		while (isRunning) {

			double time = glfwGetTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;

			Update();
			Render();

			lastTime = time;
		}
		CleanUp();
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return true;
	}
	void Application::Start() {
		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);
		Time::time = 0;
		lastTime = glfwGetTime();
	}
	void Application::Update() {
		window->OnUpdate();
	}
	void Application::Render() {

		for (Layer* layer : layerStack) {
			layer->OnUpdate();
		}
		imGuiLayer->Begin();
		for (Layer* layer : layerStack) {
			layer->OnImGuiRender();
		}
		imGuiLayer->End();
	}
	void Application::CleanUp() {
	
	}
}
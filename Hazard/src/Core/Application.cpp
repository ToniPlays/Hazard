#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Modules/GUI/LayerStack.h"

namespace Hazard {

	#define BIND_EVENT(x) std::bind(&Application::x, this, std::placeholders::_1)

	std::string Application::name = "Unnanamed Application";
	Application* Application::instance = nullptr;

	Application::Application(std::string _name)
	{
		Application::SetName(_name);
		Application::instance = this;
	}
	Application::Application() 
	{
		Application::instance = this;
	}

	Application::~Application() {
		cleanUp();
	}

	void Application::Run() {

		start();
		this->Start();
		
		while (isRunning) {

			double time = window->GetContext()->GetMSTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;
			
			update();
			render();

			lastTime = time;
		}
		cleanUp();
	}
	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(ExitApplication));
	}
	void Application::start()
	{
		isRunning = true;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetWindowTitle(Application::GetName());

		moduleHandler = ModuleHandler();
		moduleHandler.PushModule(new LayerStack());

#ifdef HZR_DEBUG
		moduleHandler.PushModule(new Logger());
#endif

		window->SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	void Application::update()
	{
		PROFILE_FN();

		moduleHandler.Update();
		Update();
		moduleHandler.LateUpdate();
		LateUpdate();

		PROFILE_FN();
	}
	void Application::render()
	{
		PROFILE_FN();

		window->OnUpdate();
		moduleHandler.OnRender();

		PROFILE_FN();
	}
	bool Application::ExitApplication(WindowCloseEvent& e) {
		isRunning = false;
		return true;
	}
	void Application::CloseApplication() {
		WindowCloseEvent e = WindowCloseEvent();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(ExitApplication));
		
	}
	void Application::cleanUp()
	{
		for (Module* module : moduleHandler.GetAll()) {
			module->Shutdown();
		}
	}
}

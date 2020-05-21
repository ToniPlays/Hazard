#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Modules/GUI/LayerStack.h"
#include "Modules/Input/Input.h"
#include "Rendering/GlobalRenderer.h"

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
	void Application::onEvent(Event& e) {

		EventDispatcher dispatcher(e);
		if(!dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::ExitApplication)))
			moduleHandler.OnEvent(e);
		OnEvent(e);

	}
	void Application::start()
	{
		isRunning = true;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetWindowTitle(Application::GetName());

		moduleHandler = ModuleHandler();
#ifdef HZR_DEBUG
		moduleHandler.PushModule(new Logger());
#endif
		moduleHandler.PushModule(new LayerStack());
		moduleHandler.PushModule(new Input());
		moduleHandler.PushModule(new GlobalRenderer());

		window->SetEventCallback(BIND_EVENT(Application::onEvent));
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
#ifdef HZR_GAME_ONLY
		moduleHandler.GetModule<GlobalRenderer>()->SceneRender();
#endif // HZR_GAME_ONLY


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

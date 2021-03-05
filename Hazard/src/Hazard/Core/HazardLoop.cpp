#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Core {

	HazardLoop* HazardLoop::instance = nullptr;

	HazardLoop::HazardLoop(Application* app) : application(app)
	{
		HazardLoop::instance = this;
	}
	HazardLoop::~HazardLoop()
	{
		delete application;
	}
	void HazardLoop::Start()
	{

		//PreInit moduleHandler
		application->PreInit();
		moduleHandler.PreInit();
		//Init application and moduleHandler
		application->Init();
		moduleHandler.Init();

		Input::Input::Init();
		double lastTime = 0;
		//Update and render while close is not requested
		while (!shouldClose) {

			//Update deltaTime and timeScale
			double time = glfwGetTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;

			//Update and render
			application->Update();
			moduleHandler.Update();

			moduleHandler.Render();

			lastTime = time;
		}
		//Close application and modules
		application->Close();
		moduleHandler.Close();
	}
	//Stop application from running
	bool HazardLoop::Quit(WindowCloseEvent& e)
	{
		Shutdown();
		return true;
	}
	void HazardLoop::Process(Event& e) {
		instance->OnEvent(e);
	}
	void HazardLoop::Shutdown()
	{
		instance->shouldClose = true;
	}
	void HazardLoop::OnEvent(Event& e)
	{
		//Dispatch WindowCloseEvent
		EventDispatcher dispatcher(e);
		if(dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit))) return;

		application->OnEvent(e);
	}
}
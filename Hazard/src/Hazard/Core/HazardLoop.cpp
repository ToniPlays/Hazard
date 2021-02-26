#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"

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
		application->Init();

		moduleHandler.PreInit();
		moduleHandler.Init();

		double lastTime = 0;
		while (!shouldClose) {

			double time = glfwGetTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;

			moduleHandler.Update();
			moduleHandler.Render();

			lastTime = time;
		}

		moduleHandler.Close();
	}
	bool HazardLoop::Quit(WindowCloseEvent& e)
	{
		instance->shouldClose = true;
		return true;
	}
	void HazardLoop::Process(Event& e) {
		instance->OnEvent(e);
	}
	void HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit));
	}
}
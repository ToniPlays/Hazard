#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "GLFW/glfw3.h"

namespace Hazard {

	HazardLoop* HazardLoop::instance;

	HazardLoop::HazardLoop(Application* app) {
		this->current = app;
		instance = this;

	}

	bool HazardLoop::OnEvent(Event& e)
	{
		if (e.GetCategoryFlags() & EventCategory::EventCategoryApplication) {
			EventDispatcher dispatcher(e);
			return dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Close));
		}
		else current->OnEvent(e);
	}
	bool HazardLoop::Close(Event& e)
	{
		shouldClose = true;
		return true;
	}

	HazardLoop::~HazardLoop()
	{
		current->OnClose();
		delete current;
	}

	void HazardLoop::Run() {
		//Awake
		moduleHandler.Awake();
		current->Awake();
		//Start
		moduleHandler.Start();
		current->Start();

		double lastTime = 0;
		while (!shouldClose) {

			PROFILE_FN();

			double time = glfwGetTime();
			Time::unscaledDeltaTime = time - lastTime;
			Time::deltaTime = Time::unscaledDeltaTime * Time::timeScale;
			Time::time = time;
			
			//Update
			moduleHandler.Update();
			current->Update();
			//Late update
			moduleHandler.LateUpdate();
			current->LateUpdate();

			//Render
			moduleHandler.OnRender();

			lastTime = time;
			PROFILE_FN_END();
		}
	}
}
#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "GLFW/glfw3.h"
#include "Hazard/Utils/Timer.h"

namespace Hazard {

	HazardLoop* HazardLoop::instance;

	HazardLoop::HazardLoop(Application* app) : current(app) {
		instance = this;
	}

	bool HazardLoop::OnEvent(Event& e)
	{
		if (e.GetCategoryFlags() & EventCategory::EventCategoryApplication) {
			EventDispatcher dispatcher(e);
			return dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Close));
		}
		else return current->OnEvent(e);

	}

	HazardLoop::~HazardLoop()
	{
		current->OnClose();
		moduleHandler.OnClose();
		delete current;
	}

	void HazardLoop::Run() {
		//Awake
		moduleHandler.Awake();
		current->Awake();
		//Start
		moduleHandler.Start();
		current->Start();

		HZR_CORE_INFO("Engine startup");

		double lastTime = 0;

		while (!current->ShouldWindowClose()) {
			//PROFILE_FN();

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
			//PROFILE_FN_END();
		}
	}
	bool HazardLoop::Close(Event& e) {
		current->shouldClose = true;
		return true;
	}
}
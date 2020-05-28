#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "GLFW/glfw3.h"

namespace Hazard {

	

	HazardLoop* HazardLoop::instance;

	ApplicationInfo HazardLoop::info = ApplicationInfo();

	HazardLoop::HazardLoop(Application* app) {
		this->current = app;
		instance = this;

	}

	bool HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Close));	
	}
	bool HazardLoop::Close(Event& e)
	{
		info.shouldClose = true;
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
		while (!info.ShouldClose()) {
			PROFILE_FN();
			double time = glfwGetTime() * 1000;
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

			PROFILE_FN();
			lastTime = time;
		}
	}
}
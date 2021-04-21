#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Core {

	HazardLoop* HazardLoop::s_Instance = nullptr;

	HazardLoop::HazardLoop(Application* app) : m_Application(app)
	{
		HazardLoop::s_Instance = this;
	}
	HazardLoop::~HazardLoop()
	{
		delete m_Application;
	}
	void HazardLoop::Start()
	{
		m_Application->PreInit();
		m_Application->Init();

		Input::Input::Init();
		double lastTime = 0;

		while (!m_ShouldClose) {

			double time = glfwGetTime();
			Time::s_UnscaledDeltaTime = time - lastTime;
			Time::s_DeltaTime = Time::s_UnscaledDeltaTime * Time::s_TimeScale;
			Time::s_Time = time;
			lastTime = time;

			m_Application->Update();
			m_ModuleHandler.Update();
			m_ModuleHandler.Render();
		}

		m_Application->Close();
		m_ModuleHandler.Close();
	}
	bool HazardLoop::Quit(WindowCloseEvent& e)
	{
		Shutdown();
		return true;
	}
	void HazardLoop::Process(Event& e) {
		s_Instance->OnEvent(e);
	}
	void HazardLoop::Shutdown()
	{
		s_Instance->m_ShouldClose = true;
	}
	void HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if(dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit))) 
			return;
		m_Application->OnEvent(e);
	}
}
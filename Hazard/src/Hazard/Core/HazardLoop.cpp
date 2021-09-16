#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "Hazard/Events/Input.h"
#include "PlatrofmUtils.h"

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
		//Engine runtime
		try 
		{
			HZR_PROFILE_SESSION_BEGIN("Startup", "c:/dev/Hazard/Logs/HazardProfile-Startup.json");
			m_Application->PreInit();
			m_Application->Init();

			Input::Input::Init();
			HZR_PROFILE_SESSION_END();

			Run();

			HZR_PROFILE_SESSION_BEGIN("Shutdown", "c:/dev/Hazard/Logs/HazardProfile-Shutdown.json");
			m_Application->Close();
			m_ModuleHandler.Close();
			HZR_PROFILE_SESSION_END();
		}
		catch (const HazardRuntimeError& error) 
		{	
			PlatformUtils::Messagebox("Runtime error", error.what());
		}
	}
	bool HazardLoop::Quit(WindowCloseEvent& e)
	{
		s_Instance->m_ShouldClose = true;
		return true;
	}
	void HazardLoop::Process(Event& e) {
		s_Instance->OnEvent(e);
	}
	void HazardLoop::Run()
	{
		HZR_PROFILE_SESSION_BEGIN("Runtime", "c:/dev/Hazard/Logs/HazardProfile-Runtime.json");
		double lastTime = 0;

		while (!m_ShouldClose)
		{
			HZR_PROFILE_SCOPE("Frame");
			double time = glfwGetTime();

			//Update Time
			Time::s_UnscaledDeltaTime = time - lastTime;
			Time::s_DeltaTime = Time::s_UnscaledDeltaTime * Time::s_TimeScale;
			Time::s_Time = time;
			lastTime = time;

			m_Application->UpdateData();
			//Update
			Input::Update();
			m_Application->Update();
			m_ModuleHandler.Update();
			//Render
			m_ModuleHandler.Render();
		}

		HZR_PROFILE_SESSION_END();
	}
	void HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if(dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit))) 
			return;
		m_Application->OnEvent(e);
	}
}
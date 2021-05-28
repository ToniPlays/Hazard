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
		try 
		{
			HZR_PROFILE_SESSION_BEGIN("Startup", "c:/dev/Hazard/Logs/HazardProfile-Startup.json");
			
			m_Application->PreInit();
			m_Application->Init();

			Input::Input::Init();
			double lastTime = 0;
			HZR_PROFILE_SESSION_END();

			HZR_PROFILE_SESSION_BEGIN("Runtime", "c:/dev/Hazard/Logs/HazardProfile-Runtime.json");
			while (!m_ShouldClose) {

				double time = glfwGetTime();
				Input::Update();
				Time::s_UnscaledDeltaTime = time - lastTime;
				Time::s_DeltaTime = Time::s_UnscaledDeltaTime * Time::s_TimeScale;
				Time::s_Time = time;
				lastTime = time;

				HZR_PROFILE_SCOPE("Frame");
				m_Application->Update();
				m_ModuleHandler.Update();
				m_ModuleHandler.Render();
				m_Application->UpdateData();
			}

			HZR_PROFILE_SESSION_BEGIN("Shutdown", "c:/dev/Hazard/Logs/HazardProfile-Shutdown.json");
			m_Application->Close();
			m_ModuleHandler.Close();
			HZR_PROFILE_SESSION_END();
		}
		catch (const HazardRuntimeError& error) 
		{	
			const char* errorCode = error.what();
			const WCHAR* pwcsName; //LPCWSTR

			// required size
			int size = MultiByteToWideChar(CP_ACP, 0, errorCode, -1, NULL, 0);
			// allocate it
			pwcsName = new WCHAR[256];
			MultiByteToWideChar(CP_ACP, 0, errorCode, -1, (LPWSTR)pwcsName, size);
			// use it....

			MessageBox(NULL, pwcsName, (LPCTSTR)L"Error", MB_OK);
			delete[] pwcsName;
		}
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
#pragma once

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Events/Input.h"
#include "PlatrofmUtils.h"
#include "Analyzer.h"

namespace Hazard::Core {

	HazardLoop* HazardLoop::s_Instance = nullptr;

	HazardLoop::HazardLoop(Application* app) : m_Application(app)
	{
		HazardLoop::s_Instance = this;
	}
	HazardLoop::~HazardLoop()
	{
		HZR_PROFILE_SESSION_BEGIN("Shutdown", "c:/dev/Hazard/Logs/HazardProfile-Shutdown.json");
		m_Application->Close();
		AssetManager::Shutdown();
		m_ModuleHandler.Close();
		delete m_Application;

		HZR_PROFILE_SESSION_END();
	}
	void HazardLoop::Start()
	{
		//Engine runtime
		try
		{
			HZR_PROFILE_SESSION_BEGIN("Startup", "c:/dev/Hazard/Logs/HazardProfile-Startup.json");
			{
				Timer timer;
				AssetManager::Init();
				m_Application->PreInit();
				Input::Init();
				m_Application->Init();

				HZR_CORE_INFO("Startup took {0} ms", timer.ElapsedMillis());
			}

			HZR_PROFILE_SESSION_END();
			HZR_PROFILE_SESSION_BEGIN("Runtime", "c:/dev/Hazard/Logs/HazardProfile-Runtime.json");
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
		HZR_PROFILE_FUNCTION("Frame");
		double time = glfwGetTime();

		//Update Time
		Time::s_UnscaledDeltaTime = time - m_LastTime;
		Time::s_DeltaTime = Time::s_UnscaledDeltaTime * Time::s_TimeScale;
		Time::s_Time = time;
		m_LastTime = time;

		m_Application->UpdateData();
		//Update
		Input::Update();
		m_Application->Update();
		m_ModuleHandler.Update();
		//Render
		m_ModuleHandler.PreRender();
		m_ModuleHandler.Render();
		m_ModuleHandler.PostRender();
	}
	void HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit)))
			return;

		if (!m_ModuleHandler.OnEvent(e))
			m_Application->OnEvent(e);
	}
}

#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "PlatformUtils.h"
#include "Application.h"
#include "Hazard/Logging/Logger.h"

namespace Hazard {

	HazardLoop* HazardLoop::s_Instance = nullptr;

	HazardLoop::HazardLoop(Application* app) : m_Application(app)
	{
		//OPTICK_THREAD("MainThread");
		HazardLoop::s_Instance = this;
		m_ModuleHandler = CreateScope<ModuleHandler>();

#ifndef HZR_RELEASE
		m_ModuleHandler->AddModule<Logging::Logger>();
#endif // HZR_RELEASE

	}
	HazardLoop::~HazardLoop()
	{
		HZR_PROFILE_SESSION_BEGIN("Shutdown", "Logs/HazardProfile-Shutdown.json");

		HZR_CORE_WARN("Shutting down");
		//AssetManager::Shutdown();
		m_Application->Close();
		//m_ModuleHandler->Close();

		HZR_PROFILE_SESSION_END();
		OPTICK_SHUTDOWN();
	}
	void HazardLoop::Start()
	{
		try 
		{
			//Preinit application to get application stack
			m_Application->PreInit();
			m_Application->Init();
		}
		catch (HazardRuntimeError& error) {
			std::cout << error.what() << std::endl;
		}
	}
	bool HazardLoop::Quit(HazardRenderer::WindowCloseEvent& e)
	{
		s_Instance->m_ShouldClose = true;
		return true;
	}
	void HazardLoop::Process(Event& e) {
		s_Instance->OnEvent(e);
	}
	void HazardLoop::Run()
	{
		//HZR_PROFILE_FRAME("MainThread");
		double time = glfwGetTime();

		//Update Time
		Time::s_UnscaledDeltaTime = time - m_LastTime;
		Time::s_DeltaTime = Time::s_UnscaledDeltaTime * Time::s_TimeScale;
		Time::s_Time = time;
		m_LastTime = time;

		m_Application->UpdateData();
		//Update
		m_Application->Update();
		m_ModuleHandler->Update();
		//Render
		m_ModuleHandler->Render();
		m_ModuleHandler->PostRender();
	}
	void HazardLoop::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(HazardLoop::Quit)))
			return;

		if (!m_ModuleHandler->OnEvent(e) && m_Application)
			m_Application->OnEvent(e);
	}
}

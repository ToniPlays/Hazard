
#include <hzrpch.h>
#include "HazardLoop.h"
#include "Hazard/Math/Time.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Hazard/Logging/Logger.h"
#include "Hazard/Assets/AssetManager.h"
#include "CommandLineArgs.h"

#include "HazardRenderer.h"

namespace Hazard
{
    HazardLoop::HazardLoop()
    {
        //OPTICK_THREAD("MainThread");
        HazardLoop::s_Instance = this;
        m_ModuleHandler = CreateScope<ModuleHandler>();
        
#ifndef HZR_RELEASE
        m_ModuleHandler->AddModule<Logging::Logger>();
#endif // HZR_RELEASE
    }
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
		
	}
    void HazardLoop::SetApplication(Application *app) {
        if(m_Application)
        {
            m_Application->Close();
            delete m_Application;
        }
        m_Application = app;
    }

	void HazardLoop::Start()
	{
		HZR_PROFILE_SESSION_BEGIN("Shutdown", "Logs/HazardProfile-Start.json");
		try
		{
			Timer timer;
			AssetManager::Init();

			//Preinit application to get application stack
			m_Application->PreInit();
			m_ModuleHandler->InitializeAll();
			m_Application->Init();

			HZR_CORE_WARN("Startup took {0} ms", timer.ElapsedMillis());
		}
		catch (HazardRuntimeError& error)
		{
			std::cout << error.what() << std::endl;
		}
		HZR_PROFILE_SESSION_END();
	}

	void HazardLoop::Close()
	{
		HZR_PROFILE_SESSION_BEGIN("Shutdown", "Logs/HazardProfile-Shutdown.json");
        AssetManager::Shutdown();
        
		m_Application->Close();
		m_ModuleHandler->Close();
        
		HZR_PROFILE_SESSION_END();
		//OPTICK_SHUTDOWN();
	}

	bool HazardLoop::Quit(WindowCloseEvent& e)
	{
		s_Instance->m_ShouldClose = true;
		return true;
	}

	void HazardLoop::Process(Event& e) 
	{
		s_Instance->OnEvent(e);
	}

	void HazardLoop::Run()
{
        HZR_PROFILE_FRAME("MainThread");
        HZR_TIMED_FUNCTION();
        
        //Update Time

		uint32_t micros =
			std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		Time::Update(micros / 1000000.0f);
        
        for(uint32_t i = 0; i < m_Application->m_MainJobs.size(); i++)
			m_Application->m_MainJobs[i]();

        m_Application->m_MainJobs.clear();
        
		//Update
		m_ModuleHandler->PreUpdate();
		m_ModuleHandler->Update();
		m_Application->Update();
		m_ModuleHandler->PostUpdate();
		//Render

		m_ModuleHandler->PreRender();
		m_ModuleHandler->Render();
		m_ModuleHandler->PostRender();

		m_FrameCount++;
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

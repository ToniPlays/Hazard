#pragma once

#include "Core.h"
#include "HazardLoop.h"
#include "ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include <Jobs.h>
#include <Jobs/JobSystem.h>

namespace Hazard 
{
	class Application 
	{
        friend class HazardLoop;
	public:
		Application() = default;
		virtual ~Application() = default;

		/// <summary>
		/// Preinit is called before the engine is started.
		/// This should be used for building the application stack
		/// </summary>
		virtual void PreInit() {};
		virtual void Init() {};
		virtual void Update() {};
		virtual void Close() {};
		virtual bool OnEvent(Event& e) { return false; };
		static uint64_t GetRenderedFrameCount()
		{
			return HazardLoop::GetCurrent().GetRenderedFrameCount();
		}
		void CreateApplicationStack(HazardCreateInfo* info);
		JobSystem& GetJobSystem() { return m_JobSystem; }
        
        void SubmitMainThread(const std::function<void()>&& function)
        {
            m_MainJobs.push_back(function);
        }

	public:
		static void Quit();
		static Application& Get() { return *HazardLoop::GetCurrent().m_Application; }

		template<typename T, typename... Args>
		static T& PushModule(Args... args) { return *HazardLoop::GetModuleHandler()->AddModule<T>(std::forward<Args>(args)...); };
		template<typename T>
		static T& GetModule() { return *HazardLoop::GetModuleHandler()->GetModule<T>(); }
		template<typename T>
		static bool HasModule() { return HazardLoop::GetModuleHandler()->HasModule<T>(); }

	private:

		JobSystem m_JobSystem;
        std::vector<std::function<void()>> m_MainJobs;
	};

	Hazard::Application* CreateApplication();
}

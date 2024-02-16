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

		void CreateApplicationStack(HazardCreateInfo* info);

		JobSystem& GetJobSystem() { return *m_JobSystem.get(); }
		void SubmitMainThread(const std::function<void()>& function)
		{
			std::scoped_lock lock(m_MainJobMutex);
			m_MainJobs.Add(function);
		}

		void Quit();

		template<typename T, typename... Args>
		T& PushModule(Args... args) { return *HazardLoop::GetModuleHandler()->AddModule<T>(std::forward<Args>(args)...); };
		template<typename T>
		T& GetModule() { return *HazardLoop::GetModuleHandler()->GetModule<T>(); }
		template<typename T>
		bool HasModule() { return HazardLoop::GetModuleHandler()->HasModule<T>(); }

	public:
		static Application& Get() { return *HazardLoop::GetCurrent().m_Application; }

	private:
		void InvokeMainThreadJobs()
		{
			m_MainJobs.Invoke();

			m_MainJobMutex.lock();
			m_MainJobs.Clear();
			m_MainJobMutex.unlock();
		}
	private:

		Scope<JobSystem> m_JobSystem;
		std::mutex m_MainJobMutex;
		Callback<void()> m_MainJobs;
	};

	Hazard::Application* CreateApplication();
}

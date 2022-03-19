#pragma once

#include "HazardLoop.h"
#include "Core.h"
#include "ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "Hazard/PerformanceProfiler.h"

namespace Hazard {

	struct ApplicationData 
	{
		float MemoryUsage = 0;
		bool Minimized = false;
		PerformanceProfiler* Profiler = new PerformanceProfiler();
	};

	class Application {
		
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
		
		void SetTitle(const std::string& title);
		void SetTitle(const char* title);
		
		void CreateApplicationStack(HazardCreateInfo* info);
		void UpdateData();

	public:
		static void Quit();
		static ApplicationData& GetData() { return s_Data; }

		template<typename T, typename... Args>
		static T& PushModule(Args... args) { return *Core::HazardLoop::GetModuleHandler()->AddModule<T>(std::forward<Args>(args)...); };
		template<typename T>
		static T& GetModule() { return *Core::HazardLoop::GetModuleHandler()->GetModule<T>(); }
		template<typename T>
		static bool HasModule() { return Core::HazardLoop::GetModuleHandler()->HasModule<T>(); }

	private:
		static ApplicationData s_Data;
	};
	Hazard::Application* CreateApplication();
}

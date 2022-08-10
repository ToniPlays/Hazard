#pragma once

#include "Core.h"
#include "HazardLoop.h"
#include "ApplicationCreateInfo.h"
#include "Hazard/Module.h"

namespace Hazard {

	struct ApplicationData 
	{
		float MemoryUsage = 0;
		bool Minimized = false;
	};

	class Application {
		
	public:
		Application() = default;
		virtual ~Application() = default;

		/// <summary>
		/// Preinit is called before the engine is started.
		/// This should be used for building the application stack
		/// </summary>
		/// 
		virtual void PreInit() {};
		virtual void Init() {};
		virtual void Update() {};
		virtual void Close() {};
		virtual bool OnEvent(Event& e) { return false; };
		
		void CreateApplicationStack(HazardCreateInfo* info);
		void UpdateData();

	public:
		static void Quit();
		static ApplicationData& GetData() { return s_Data; }
		static Application& Get() { return *HazardLoop::GetCurrent().m_Application; }

		template<typename T, typename... Args>
		static T& PushModule(Args... args) { return *HazardLoop::GetModuleHandler()->AddModule<T>(std::forward<Args>(args)...); };
		template<typename T>
		static T& GetModule() { return *HazardLoop::GetModuleHandler()->GetModule<T>(); }
		template<typename T>
		static bool HasModule() { return HazardLoop::GetModuleHandler()->HasModule<T>(); }

	private:
		static ApplicationData s_Data;
	};
	Hazard::Application* CreateApplication();
}

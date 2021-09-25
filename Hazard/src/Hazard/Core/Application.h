#pragma once

#include "Core.h"
#include "ApplicationCreateInfo.h"
#include "Hazard/Module.h"

namespace Hazard {

	struct ApplicationData 
	{
		float memoryUsage = 0;
	};

	class Application {

	public:
		Application() = default;
		virtual ~Application() = default;

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
		static ApplicationData GetData() { return s_Data; }

		template<typename T, typename... Args>
		static T& PushModule(Args... args) { return Core::HazardLoop::GetCurrent().m_ModuleHandler.AddModule<T>(std::forward<Args>(args)...); };
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetCurrent().m_ModuleHandler.GetModule<T>(); }
		template<typename T>
		static bool HasModule() { return Core::HazardLoop::GetCurrent().m_ModuleHandler.HasModule<T>(); }

	private:
		static ApplicationData s_Data;
	};
	Hazard::Application* CreateApplication();
}
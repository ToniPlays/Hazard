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
		Application() {};
		~Application() {};

		virtual void PreInit() {};
		virtual void Init() {};
		virtual void Update() {};
		virtual void Close() {};
		virtual bool OnEvent(Event& e) { return false; };
		
		void SetTitle(const std::string& title);
		void SetTitle(const char* title);
		void UpdateData();
		
		void CreateApplicationStack(HazardCreateInfo* info);

	public:
		static void Quit();
		static ApplicationData GetData() { return s_Data; }

		template<typename T>
		static T& PushModule() { return Core::HazardLoop::GetCurrent().PushModule<T>(); };
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetModule<T>(); }
		template<typename T>
		static bool HasModule() { return Core::HazardLoop::HasModule<T>(); }

	private:
		static ApplicationData s_Data;
	};

	Hazard::Application* CreateApplication();
}
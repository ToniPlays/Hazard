#pragma once

#include "Core.h"
#include "Hazard/Module/Module.h"

namespace Hazard {

	class Application {
		friend class HazardLoop;
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
		void SetWindowIcon(const char* windowIcon, const char* barIcon);
		
	public:
		static void Quit();

		template<typename T>
		T& PushModule() { return Core::HazardLoop::GetCurrent().PushModule<T>(); };
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetModule<T>(); }
		template<typename T>
		static T& GetModule(bool& found) { return Core::HazardLoop::GetModule<T>(found); }
	};

	Hazard::Application* CreateApplication();
}
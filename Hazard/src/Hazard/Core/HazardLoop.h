#pragma once

#include "Core.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard::Core 
{
	class HazardLoop {
		friend class Application;
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);
		bool ShouldClose() { return m_ShouldClose; }
		void Run();

	public:
		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }

	private:
		Application* m_Application = nullptr;
		Scope<Module::ModuleHandler> m_ModuleHandler;

		bool m_ShouldClose = false;
		double m_LastTime = 0;
	private:
		static HazardLoop* s_Instance;
	};
}
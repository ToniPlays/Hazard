#pragma once

#include "Core.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard {
	class Application;
}

namespace Hazard::Core 
{
	class HazardLoop {
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);
		bool ShouldClose() { return m_ShouldClose; }
		void Run();

		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }

		Application* m_Application = nullptr;
		Scope<Module::ModuleHandler> m_ModuleHandler;

		bool m_ShouldClose = false;
		double m_LastTime = 0;
		static HazardLoop* s_Instance;
	};
}
#pragma once

#include "Core.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard::Core {

	class HazardLoop {
		friend class Application;
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);
	public:
		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }

	private:
		void Run();

	private:
		Application* m_Application = nullptr;
		Module::ModuleHandler m_ModuleHandler;

		bool m_ShouldClose = false;
	private:
		static HazardLoop* s_Instance;
	};
}
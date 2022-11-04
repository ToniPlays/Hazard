#pragma once

#include "Core.h"
#include "UtilityCore.h"
#include "ApplicationEvent.h"
#include "Backend/Core/Events/Events.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard 
{
	class Application;

	class HazardLoop {
		friend class Application;
        
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		void Close();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);
		bool ShouldClose() { return m_ShouldClose; }
		void Run();

		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }
        static ModuleHandler* GetModuleHandler() { return s_Instance->m_ModuleHandler.get(); }

	private:
		Application* m_Application = nullptr;
		Scope<ModuleHandler> m_ModuleHandler;

		bool m_ShouldClose = false;
		static HazardLoop* s_Instance;
	};
}

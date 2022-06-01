#pragma once

#include "Core.h"
#include "UtilityCore.h"
#include "ApplicationEvent.h"
#include "Backend/Core/Events.h"
#include "Hazard/ModuleHandler.h"
#include "Backend/Core/Events.h"
#include "Backend/Core/Events.h"

namespace Hazard {
	class Application;
}

namespace Hazard::Core 
{
	class HazardLoop {
		friend class Application;
        
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(HazardRenderer::WindowCloseEvent& e);
		void OnEvent(Event& e);
		bool ShouldClose() { return m_ShouldClose; }
		void Run();

		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }
        static Module::ModuleHandler* GetModuleHandler() { return s_Instance->m_ModuleHandler.get(); }

	private:
		Application* m_Application = nullptr;
		Scope<Module::ModuleHandler> m_ModuleHandler;

		bool m_ShouldClose = false;
		double m_LastTime = 0;
		static HazardLoop* s_Instance;
	};
}

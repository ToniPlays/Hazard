#pragma once
#include "Core.h"
#include "Application.h"
#include "Events/Event.h"
#include "Hazard/Modules/ModuleHandler.h"

namespace Hazard {

	class HAZARD_API HazardLoop {
		friend class Application;
	public:

		HazardLoop(Application* app);
		~HazardLoop();
		void Run();
		bool OnEvent(Event& e);

		static HazardLoop& GetCurrent() { return *instance; }
		
	private:
		bool shouldClose = false;
		static HazardLoop* instance;
		bool Close(Event& e);
		Application* current;
		ModuleHandler moduleHandler;
	};
}
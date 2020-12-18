#pragma once
#include "Core.h"
#include "Application.h"
#include "Events/Event.h"
#include "Hazard/ModuleHandler.h"

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

		static HazardLoop* instance;
		Application* current;
		ModuleHandler moduleHandler;

		bool Close(Event& e);
	};
}
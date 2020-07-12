#pragma once
#include "Core.h"
#include "Application.h"
#include "ApplicationInfo.h"
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

		static ApplicationInfo& GetAppInfo() { return info; }
		static HazardLoop& GetCurrent() { return *instance; }

	private:

		static HazardLoop* instance;
		bool Close(Event& e);
		Application* current;
		ModuleHandler moduleHandler;
		static ApplicationInfo info;
	};
}
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
		static ApplicationInfo& GetAppInfo() { return info; }
		void Run();
		static HazardLoop& GetCurrent() { return *instance; }
		bool OnEvent(Event& e);

	private:

		static HazardLoop* instance;
		bool Close(Event& e);
		Application* current;
		ModuleHandler moduleHandler;
		static ApplicationInfo info;
	};
}
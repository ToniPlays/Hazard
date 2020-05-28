#pragma once

#include "Core.h"
#include "IApplication.h"
#include "Hazard/Modules/Module.h"
#include "Hazard/Modules/ModuleHandler.h"

namespace Hazard {
	class HAZARD_API Application : public IApplication {
	public:
		Application() {};
		~Application() {};

		void PushModule(Module* module) {
			ModuleHandler::PushModule(module);
		};
	};



	Hazard::Application* CreateApplication();
}
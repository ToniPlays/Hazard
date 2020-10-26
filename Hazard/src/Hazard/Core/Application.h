#pragma once

#include "Core.h"
#include "IApplication.h"
#include "Hazard/Modules/Module.h"


namespace Hazard {
	class HAZARD_API Application : public IApplication {
	public:
		Application() {};
		~Application() {};

		void PushModule(Module* module);
		void SetTitle(std::string& title);
		static Application& GetApplication();
	};

	Hazard::Application* CreateApplication();
}
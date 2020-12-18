#pragma once

#include "Core.h"
#include "IApplication.h"
#include "Hazard/Module.h"


namespace Hazard {
	class HAZARD_API Application : public IApplication {
		friend class HazardLoop;
	public:
		Application() {};
		~Application() {};

		void PushModule(Module* module);
		void SetTitle(const char* title);
		static Application& GetApplication();

		bool ShouldWindowClose() { return shouldClose; }

	private:
		
		bool shouldClose = false;
	};

	Hazard::Application* CreateApplication();
}
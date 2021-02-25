#pragma once

#include "Core.h"
#include "Hazard/Module/Module.h"

namespace Hazard {

	class HAZARD_API Application {
		friend class HazardLoop;
	public:
		Application() {};
		~Application() {};

		virtual void Init() {};
		
		void SetTitle(std::string title);
		void SetTitle(const char* title);

		template<typename T>
		void PushModule() {
			Core::HazardLoop::GetCurrent().PushModule<T>();
		};
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetModule<T>(); }
		
	};
	Hazard::Application* CreateApplication();
}
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
		virtual void Update() {};
		virtual void Close() {};

		template<typename T>
		T& PushModule() {
			return Core::HazardLoop::GetCurrent().PushModule<T>();
		};
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetModule<T>(); }
		template<typename T>
		static T& GetModule(bool& found) { return Core::HazardLoop::GetModule<T>(found); }
		
	};
	Hazard::Application* CreateApplication();
}
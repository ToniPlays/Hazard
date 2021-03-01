#pragma once

#include "Core.h"
#include "Hazard/Module/Module.h"

namespace Hazard {

	/*
	Hazard runnable application
	Necessary for linking Hazard and client app
	*/

	class HAZARD_API Application {
		friend class HazardLoop;
	public:
		Application() {};
		~Application() {};

		//Application events;
		virtual void PreInit() {};
		virtual void Init() {};
		virtual void Update() {};
		virtual void Close() {};

		virtual bool OnEvent(Event& e) { return false; };

		//Push new module to module stack
		template<typename T>
		T& PushModule() {
			return Core::HazardLoop::GetCurrent().PushModule<T>();
		};

		//Get specified module type from module stack
		template<typename T>
		static T& GetModule() { return Core::HazardLoop::GetModule<T>(); }

		//Boolean set to true if module exists, otherwise no modification
		template<typename T>
		static T& GetModule(bool& found) { return Core::HazardLoop::GetModule<T>(found); }
		
	};
	//External definition, called when engine is starting
	Hazard::Application* CreateApplication();
}
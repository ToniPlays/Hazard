#pragma once

#include "Core.h"

#include "Hazard/Core/Application.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/Module/ModuleHandler.h"

namespace Hazard::Core {

	/*HazardLoop runs the application and is part of the application API
	All modules can be accessed from within HazardLoop and GetCurrent();
	*/
	class HazardLoop {
		friend class Application;
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		//Starts application
		void Start();
		//Closes the application
		bool Quit(WindowCloseEvent& e);
		//Processses events
		void OnEvent(Event& e);

	public:
		/*Static Processing of events, can be accessed anywhere
		Directed to instance OnEvent
		*/
		static void Process(Event& e);
		//Get current HazardLoop instance
		static HazardLoop& GetCurrent() { return *instance; }

	private:
		//Push module to module stack
		template<typename T>
		static T& PushModule() {
			return instance->moduleHandler.AddModule<T>();
		};
		//Get module from module stack
		template<typename T>
		static T& GetModule() { return instance->moduleHandler.GetModule<T>(); };

		template<typename T>
		static T& GetModule(bool& found) { return instance->moduleHandler.GetModule<T>(found); };

		void Shutdown();

	private:
		Application* application = nullptr;
		Module::ModuleHandler moduleHandler;
		bool shouldClose = false;
		static HazardLoop* instance;
	};
}
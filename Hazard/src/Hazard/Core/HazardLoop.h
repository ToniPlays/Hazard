#pragma once

#include "Core.h"

#include "Hazard/Core/Application.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/Module/ModuleHandler.h"

namespace Hazard::Core {
	class HazardLoop {
	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(WindowCloseEvent& e);

		void OnEvent(Event& e);

	public:
		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *instance; }

		template<typename T>
		static T& PushModule() {
			return instance->moduleHandler.AddModule<T>();
		};

		template<typename T>
		static T& GetModule() { return instance->moduleHandler.GetModule<T>(); };
		template<typename T>
		static T& GetModule(bool& found) { return instance->moduleHandler.GetModule<T>(found); };

	private:
		Application* application = nullptr;
		Module::ModuleHandler moduleHandler;
		bool shouldClose = false;

	private:
		static HazardLoop* instance;
	};
}
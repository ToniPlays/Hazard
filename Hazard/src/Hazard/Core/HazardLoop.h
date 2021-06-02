#pragma once

#include "Core.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/ModuleHandler.h"

namespace Hazard::Core {

	class HazardLoop {
		friend class Application;

	public:
		HazardLoop(Application* app);
		~HazardLoop();

		void Start();
		bool Quit(WindowCloseEvent& e);
		void OnEvent(Event& e);

	public:
		static void Process(Event& e);
		static HazardLoop& GetCurrent() { return *s_Instance; }

		template<typename T>
		static T& PushModule() { return s_Instance->m_ModuleHandler.AddModule<T>(); };
		template<typename T>
		static T& GetModule() { return s_Instance->m_ModuleHandler.GetModule<T>(); };
		template<typename T>
		static bool HasModule() { return s_Instance->m_ModuleHandler.HasModule<T>(); };

	private:
		void Run();
		void Shutdown();

	private:
		Application* m_Application = nullptr;
		Module::ModuleHandler m_ModuleHandler;

		bool m_ShouldClose = false;
		static HazardLoop* s_Instance;
	};
}
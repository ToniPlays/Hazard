#pragma once
#include "Core.h"
#include "HazardInterface.h"
#include "Platform/Window.h"
#include "Modules/Module/ModuleHandler.h"
#include "Events/ApplicationEvent.h"

namespace Hazard {

	class HAZARD_API Application : public HazardInterface {

	public:
		Application(std::string _name);
		Application();
		~Application();

	public:
		void Run();
		Window& GetWindow() { return *window; }

		static std::string GetName() { return name; }
		static void SetName(std::string _name) { Application::name = _name + " | Hazard"; }
		static Application& GetCurrent() { return *instance; }
		
		ModuleHandler& GetModuleHandler() { return *&moduleHandler; };
		void CloseApplication();
	private:
		void OnEvent(Event& e);
		void start();
		void update();
		void render();
		void cleanUp();
		bool ExitApplication(WindowCloseEvent& e);

	private:
		double lastTime = 0;
		ModuleHandler moduleHandler;
		std::unique_ptr<Window> window;
		static std::string name;
		static Application* instance;
		bool isRunning = false;
	};

	Hazard::Application* CreateApplication();
}
#pragma once
#include <hzrpch.h>
#include "Platform/Window.h"
#include "Core/EventSystem/ApplicationEvent.h"
#include "Core/Layers/LayerStack.h"
#include "Rendering/Renderer2D.h"

enum class UpdateType {
	PollEvents = 0,
	WaitEvents = 1
};

namespace Hazard {

	class HAZARD_API Application {

	public:
		Application(std::string name);
		virtual ~Application();
		static std::string getAppName() { return name; }
		void Run();
		
		void OnEvent(Event& e);
		inline static Application& Get() {	return *instance; }
		inline Window& GetWindow() { return *window; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		UpdateType getUpdateType() { return type; }
		static void Close() { Application::Get().isRunning = false; }
		
		

	//Client side
	public:
		virtual void Start() {}

	private:

		UpdateType type = UpdateType::PollEvents;
		static std::string name;
		bool isRunning = true;
		std::unique_ptr<Window> window;
		LayerStack layerStack;
		float lastTime = 0;
		static Application* instance;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		void APPStart();
		void APPUpdate();
		void APPRender();
		void APPCleanUp();

	//Temporary
	private:
		Renderer2D* renderer;
	};

	Hazard::Application* CreateApplication();
}
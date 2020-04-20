#pragma once
#include <hzrpch.h>
#include "Core/Window/Window.h"
#include "Core/EventSystem/ApplicationEvent.h"
#include "Core/Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

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

	private:

		UpdateType type = UpdateType::WaitEvents;
		static std::string name;
		bool isRunning = true;
		std::unique_ptr<Window> window;
		LayerStack layerStack;
		ImGuiLayer* imGuiLayer;

		static Application* instance;

		bool OnWindowClose(WindowCloseEvent& e);

		void Start();
		void Update();
		void Render();
		void CleanUp();
	};

	Hazard::Application* CreateApplication();
}
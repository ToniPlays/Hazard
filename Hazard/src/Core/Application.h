#pragma once
#include <hzrpch.h>
#include "Platform/Window.h"
#include "Core/EventSystem/ApplicationEvent.h"
#include "Core/Layers/LayerStack.h"
#include "Rendering/Renderer2D.h"
#include "Rendering/RenderAPI/GraphicsContext.h"
#include "ApplicationInfo.h"


enum class UpdateType {
	PollEvents = 0,
	WaitEvents = 1
};

namespace Hazard {

	class HAZARD_API Application {

	public:

		Application(std::string name);
		virtual ~Application();

		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void OnEvent(Event& e);

		inline static Application& Get() {	return *instance; }
		inline Window& GetWindow() { return *window; }
		UpdateType getUpdateType() { return type; }
		static void Close() { Application::Get().isRunning = false; }
		static ApplicationInfo& GetInfo() { return *info; }
		

	//Client side
	public:
		virtual void Start() {}
	private:

		UpdateType type = UpdateType::PollEvents;
		bool isRunning = true;
		std::unique_ptr<Window> window;
		LayerStack layerStack;
		float lastTime = 0;

		static Application* instance;
		static ApplicationInfo* info;

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
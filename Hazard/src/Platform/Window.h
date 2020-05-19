#pragma once
#include "Core/Core.h"
#include "GraphicsContext.h"
#include "Events/Event.h"

namespace Hazard {

	struct WindowProps {

		std::string Title;
		std::string Platform;
		std::string Renderer;

		bool VSync = false;
		unsigned int Width;
		unsigned int Height;

		std::function<void(Event&)> EventCallback;

		WindowProps(const std::string& title = "Hazard",
			std::string platform = "Undefined platform",
			std::string renderer = "Undefined renderer",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Platform(platform), Renderer(renderer), Width(width), Height(height) {}
	};

	class HAZARD_API Window {

	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate() = 0;
		virtual void SetClearColor(Color color) const = 0;
		virtual void SetWindowTitle(std::string title) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool IsVSync() const = 0;
		virtual WindowProps GetWindowInfo() { return windowData; }

		virtual Color GetClearColor() const = 0;
		static Window* Create(WindowProps& props = WindowProps());
		GraphicsContext* GetContext() const { return context; }

	protected:
		GraphicsContext* context = nullptr;
		WindowProps windowData;
	};
}
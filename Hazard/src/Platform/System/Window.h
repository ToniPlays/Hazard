#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Color.h"
#include "Platform/Rendering/GraphicsContext.h"
#include <iostream>

namespace Hazard {

	struct WindowProps {

		const char* Title;
		const char* Platform;
		std::string Renderer;

		bool VSync = false;
		bool focus = true;
		bool maximized = false;
		bool minimized = false;
		int Width;
		int Height;

		std::function<void(Event&)> EventCallback;

		WindowProps(const std::string& title = "Hazard",
			const char* platform = "Undefined platform",
			std::string renderer = "Undefined renderer",
			unsigned int width = 1280,
			unsigned int height = 720, bool maxim = true)
			: Title(title.c_str()), Platform(platform), Renderer(renderer), Width(width), Height(height), maximized(maxim) {}
	};

	class HAZARD_API Window {

	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate(Color color = Color("#222222")) = 0;
		virtual void SetWindowTitle(const char* title) = 0;
		virtual void SetWindowIcon(const char* smallIcon, const char* bigIcon) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual bool IsVSync() const = 0;
		virtual bool IsFocused() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual WindowProps GetWindowInfo() { return windowData; }

		static Window* Create(WindowProps& props = WindowProps());
		GraphicsContext* GetContext() const { return context; }

	protected:
		GraphicsContext* context = nullptr;
		WindowProps windowData;
	};
}
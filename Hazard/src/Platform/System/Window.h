#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"
#include "Platform/Rendering/GraphicsContext.h"

#include <iostream>

namespace Hazard::Rendering {

	class GraphicsContext;

	struct WindowProps {

		std::string Title;
		const char* Platform;
		DeviceSpec deviceSpecs;

		bool VSync = false;
		bool focus = true;
		bool maximized = false;
		bool minimized = false;
		bool fullscreen = false;
		int Width = 0;
		int Height = 0;
		uint32_t ImagesInFlight = 0;

		std::function<void(Event&)> EventCallback;
		WindowProps() = default;
	};

	class Window {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnBeginFrame() = 0;
		virtual void OnEndFrame() = 0;
		virtual void SetWindowTitle(const char* title) = 0;
		virtual void SetWindowIcon(uint32_t count, const char** images) = 0;
		virtual void SetPosition(glm::vec2 position, glm::vec2 dragPoint) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetFullscreen(bool enabled) = 0;
		virtual void SetMaximized(bool enabled) = 0;
		virtual void SetMinimized(bool enabled) = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual bool IsVSync() const = 0;
		virtual bool IsFocused() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsFullscreen() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual WindowProps GetWindowInfo() = 0;
		virtual GraphicsContext* GetContext() const = 0;

		static Window* Create(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
	};
}
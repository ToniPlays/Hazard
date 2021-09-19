#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"
#include "Platform/Rendering/GraphicsContext.h"

#include <iostream>

namespace Hazard::Rendering {

	struct WindowProps {

		const char* Title;
		const char* Platform;
		DeviceSpec deviceSpecs;

		bool VSync = false;
		bool focus = true;
		bool maximized = false;
		bool minimized = false;
		bool fullscreen = false;
		int Width = 0;
		int Height = 0;

		std::function<void(Event&)> EventCallback;
		WindowProps() = default;
	};

	class Window {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate(const Color& color = Color("#000000")) = 0;
		virtual void SetWindowTitle(const char* title) = 0;
		virtual void SetWindowIcon(uint32_t count, std::string* images) = 0;
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

		virtual WindowProps GetWindowInfo() { return m_WindowData; }

		static Window* Create(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
		GraphicsContext* GetContext() const { return m_Context; }

	protected:
		GraphicsContext* m_Context = nullptr;
		WindowProps m_WindowData;
	};
}
#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/System/Window.h"
#include "GLFW/glfw3.h"
#include "Hazard/Math/Color.h"

namespace Hazard::Rendering {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
		virtual ~WindowsWindow();
		
		void OnUpdate(Color color) override;
		void SetWindowTitle(const char* title) override;
		void SetWindowIcon(uint32_t count, std::string* images) override;

		void SetVSync(bool enabled) override {
			m_WindowData.VSync = enabled;
			glfwSwapInterval(enabled);
		};
		void SetMaximized(bool maximized) {
			m_WindowData.maximized = maximized;
		}
		void SetFullscreen(bool fullscreen);
		void* GetNativeWindow() const { return m_Window; }
		void SetEventCallback(const EventCallbackFn& callback) {
			m_WindowData.EventCallback = callback;
		}
		unsigned int GetWidth() const { return m_WindowData.Width; }
		unsigned int GetHeight() const { return m_WindowData.Height; }
		
		bool IsVSync() const { return m_WindowData.VSync; }
		bool IsFocused() const { return m_WindowData.focus; }
		bool IsMinimized() const { return m_WindowData.minimized; }
		bool IsFullscreen() const { return m_WindowData.fullscreen; }
		WindowProps GetWindowInfo() { return m_WindowData; }

	private:

		GLFWwindow* m_Window = nullptr;
		void SetCallbacks();
	};
}
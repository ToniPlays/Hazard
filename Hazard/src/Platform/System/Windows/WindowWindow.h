#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/System/Window.h"
#include "GLFW/glfw3.h"
#include "Hazard/Math/Color.h"

namespace Hazard::Rendering {

	class HAZARD_API WindowsWindow : public Window {
	public:
		WindowsWindow(WindowProps& props);
		virtual ~WindowsWindow();
		
		void OnUpdate(Color color) override;
		void SetWindowTitle(const char* title) override;
		void SetWindowIcon(const char* smallIcon, const char* bigIcon) override;

		void SetVSync(bool enabled) override {
			m_WindowData.VSync = enabled;
			glfwSwapInterval(enabled);
		};
		void SetFullscreen(bool fullscreen) {
			m_WindowData.fullscreen = fullscreen;
		}
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
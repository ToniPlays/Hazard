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

		void OnUpdate() override;
		void SetWindowTitle(const char* title) override;
		void SetWindowIcon(uint32_t count, std::string* images) override;
		void SetPosition(glm::vec2 position, glm::vec2 dragPoint);

		void SetVSync(bool enabled) override {
			m_WindowData.VSync = enabled;
			glfwSwapInterval(enabled);
		};
		void SetMaximized(bool maximized) override {
			m_WindowData.maximized = maximized;
			if (maximized)
				glfwMaximizeWindow(m_Window);
			else
				glfwRestoreWindow(m_Window);
		}
		void SetMinimized(bool minimized) override {
			m_WindowData.minimized = minimized;
			glfwIconifyWindow(m_Window);
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
		bool IsMaximized() const { return m_WindowData.maximized; }
		WindowProps GetWindowInfo() { return m_WindowData; }

	private:

		GLFWwindow* m_Window = nullptr;
		void SetCallbacks();
	};
}
#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/System/Window.h"
#include "GLFW/glfw3.h"

namespace Hazard {
	class HAZARD_API WindowsWindow : public Window {
	public:
		WindowsWindow(WindowProps& props);
		virtual ~WindowsWindow();
		
		void OnUpdate(Color color) override;
		void SetWindowTitle(std::string title) override;
		void SetWindowIcon(const char* smallIcon, const char* bigIcon) override;

		void SetVSync(bool enabled) override {
			windowData.VSync = enabled;
			glfwSwapInterval(enabled);
		};
		void* GetNativeWindow() const { return window; }
		void SetEventCallback(const EventCallbackFn& callback) {
			windowData.EventCallback = callback;
		}
		unsigned int GetWidth() const { return windowData.Width; }
		unsigned int GetHeight() const { return windowData.Height; }
		
		bool IsVSync() const { return windowData.VSync; }
		bool IsFocused() const { return windowData.focus; }
		WindowProps GetWindowInfo() { return windowData; }

	private:
		GLFWwindow* window = nullptr;
		void SetCallbacks();
	};
}
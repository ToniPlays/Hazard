#pragma once
#include "Core/Core.h"
#include "GLFW/glfw3.h"

namespace Hazard {

	class HAZARD_API WindowsWindow : public Window {
	public:

		WindowsWindow(WindowProps& props);

		void OnUpdate();
		void SetClearColor(Color color) const;
		void SetWindowTitle(std::string title);

		void SetVSync(bool enabled) {
			glfwSwapInterval(enabled);
			windowData.VSync = enabled;
		}

		bool IsVSync() const { return windowData.VSync; }

		unsigned int GetWidth() const { return windowData.Width; }
		unsigned int GetHeight() const { return windowData.Height; }
		Color GetClearColor() const;
		void* GetNativeWindow() const { return window; }

		inline void SetEventCallback(const EventCallbackFn& callback) {
			windowData.EventCallback = callback;
		}

	private:
		void SetCallbacks();
		void Init(WindowProps& props);
		GLFWwindow* window;
	};
}
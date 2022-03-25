
#pragma once

#include "Core/Core.h"
#ifdef HZR_PLATFORM_WINDOWS

#include "Core/Window.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer {

	class WindowsWindow : public Window {
		friend class Window;
	public:
		WindowsWindow(HazardRendererCreateInfo* createInfo);
		virtual ~WindowsWindow();

		virtual void Show() const override;
		virtual void Hide() const override;
		virtual void Close() override;

		virtual void BeginFrame() override;
		virtual void Present() override;

		void SetWindowTitle(const std::string& title) override;
		void SetFullscreen(bool fullscreen);

		void SetMaximized(bool maximized) override
		{
			m_WindowData.maximized = maximized;
			maximized ? glfwMaximizeWindow(m_Window) : glfwRestoreWindow(m_Window);
		}

		void SetMinimized(bool minimized) override
		{
			m_WindowData.minimized = minimized;
			glfwIconifyWindow(m_Window);
		}

		void SetVSync(bool enabled) override
		{
			m_WindowData.VSync = enabled;
			glfwSwapInterval(enabled);
		};

		void SetPosition(glm::vec2 position) override;
		void SetWindowIcon(uint32_t count, const char** images) override;

		void* GetNativeWindow() const override { return m_Window; }

		unsigned int GetWidth() const { return m_WindowData.Width; }
		unsigned int GetHeight() const { return m_WindowData.Height; }

		bool IsVSync() const override { return m_WindowData.VSync; }
		bool IsFocused() const { return m_WindowData.focus; }
		bool IsMinimized() const override { return m_WindowData.minimized; }
		bool IsFullscreen() const override { return m_WindowData.fullscreen; }
		bool IsMaximized() const override { return m_WindowData.maximized; }
		glm::vec2 GetPosition() override;

		WindowProps GetWindowInfo() override { return m_WindowData; }

		GraphicsContext* GetContext() const { return m_Context; };

	private:
		WindowProps m_WindowData;
		GraphicsContext* m_Context;
		GLFWwindow* m_Window = nullptr;
		static inline RendererMessageCallback s_DebugCallback;
		void SetCallbacks();

	};
}
#endif

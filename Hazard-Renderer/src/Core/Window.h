#pragma once

#include "RenderContextCreateInfo.h"
#include "GraphicsContext.h"
#include "Event.h"

#include <functional>
#include <iostream>
#include "glm/glm.hpp"

namespace HazardRenderer
{
	class GraphicsContext;

	class Window;
	class Swapchain;

    enum class Orientation : uint8_t
    {
        LandscapeLeft = BIT(0),
        LandscapeRight = BIT(1),
        Portrait = BIT(2),
        PortraitInverse = BIT(3),
        AnyPortrait = Portrait | PortraitInverse,
        AnyLandscape = LandscapeLeft | LandscapeRight,
    };

    struct SafeArea
    {
        uint32_t Top;
        uint32_t Right;
        uint32_t Bottom;
        uint32_t Left;
    };

	struct WindowProps
    {
		friend class Window;

		std::string Title;
		const char* Platform;
        RenderAPI SelectedAPI = RenderAPI::Auto;

		uint32_t Width = 0;
		uint32_t Height = 0;
        uint8_t Orientation = (uint8_t)Orientation::LandscapeLeft;
		float RefreshRate = 0.0f;

		bool VSync = false;
		bool focus = true;
		bool Maximized = false;
		bool Minimized = false;
		bool Fullscreen = false;
		bool HasTitleBar = true;

		uint32_t ImagesInFlight = 0;
        glm::vec2 FramebufferScale = { 1.0f, 1.0f };
		Window* Window;

		std::function<void(Event&)> EventCallback;
		WindowProps() = default;
	};

	struct Resolution 
	{
		uint32_t Width;
		uint32_t Height;
		float RefreshRate;
	};

	class Window
    {

	public:

		virtual ~Window() {};
		virtual void Show() const = 0;
		virtual void Hide() const = 0;
		virtual void Close() = 0;

		virtual void BeginFrame() = 0;
		virtual void Present() = 0;

		virtual void SetWindowTitle(const std::string& title) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetFullscreen(bool enabled) = 0;
		virtual void SetMaximized(bool enabled) = 0;
		virtual void SetMinimized(bool enabled) = 0;

		virtual void SetPosition(glm::vec2 position) = 0;
		virtual void SetWindowIcon(uint32_t count, const char** images) = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsFullscreen() const = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool IsFocused() const = 0;
		virtual glm::vec2 GetPosition() = 0;
		virtual std::vector<Resolution> GetAvailableResolutions() const = 0;
		virtual void SetResolution(const Resolution& resolution) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
        virtual SafeArea GetSafeArea() const = 0;

		virtual const WindowProps& GetWindowInfo() = 0;
		virtual GraphicsContext* GetContext() const = 0;
		virtual Ref<Swapchain> GetSwapchain() = 0;
		virtual void AddDebugCallback(const RendererMessageCallback& callback) = 0;

		static bool IsRenderAPISupported(const RenderAPI& api);
		static void SendDebugMessage(const RenderMessage& message);

		static Window* Create(HazardRendererCreateInfo* info);
	};
}

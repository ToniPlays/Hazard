#pragma once

#include <hzrpch.h>
#include "Core/EventSystem/Event.h"
#include "Utils/Color.h"
#include "Rendering/RenderAPI/GraphicsContext.h"

namespace Hazard {

	struct WindowProps {

		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazard",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class Window {
	public:
		
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetClearColor(Color color) const = 0;
		virtual Color GetClearColor() const = 0;
		virtual GraphicsContext& GetContext() const = 0;
		//Attribs
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		static Window* Create(const WindowProps& props = WindowProps());
	};
}
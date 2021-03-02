#pragma once
#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"


namespace Hazard::Rendering {

	enum class RenderAPI { 
		Auto = 0,
		OpenGL,
		Vulkan
	};

	class RenderContext : public Module::Module {

	public:
		RenderContext(RenderAPI api = RenderAPI::OpenGL);
		~RenderContext();

		void Init();
		void Update() override;
		void Close();
		bool IsAvailable() { return true; }

		void SetClearColor(Color color) { clearColor = color; };

		RenderAPI GetCurrentAPI() { return currentAPI; }
		GraphicsContext& GetContext() { return *window->GetContext(); }
		Window& GetWindow() { return *window; }

	private:
		RenderAPI currentAPI;
		Window* window = nullptr;
		Color clearColor;
	private:
		void Process(Event& e);
	};
}
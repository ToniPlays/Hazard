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

		RenderAPI GetCurrentAPI() { return currentAPI; }
		const char* ApiAsString(RenderAPI api);

		Window& GetWindow() { return *window; }

	private:
		RenderAPI currentAPI;
		Window* window = nullptr;

	private:
		void Process(Event& e);
	};
}
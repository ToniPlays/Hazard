#pragma once

#include "Hazard/Module.h"
#include "Platform/System/Window.h"
#include "Hazard/Rendering/Textures/RenderTexture.h"

namespace Hazard {

	struct RendererStats {
		uint32_t width, height;
	};

	class RenderEngine : public Module {
	public:

		RenderEngine();
		~RenderEngine();

		bool OnEnabled();
		bool OnDisabled();

		void Render();
		void ProcessEvent(Event& e);

		void OnViewResized(float w, float h);

		void OnError(ErrorData& data);
		void SetTitle(const char* title);

		RendererStats& GetRenderStats() { return stats; }
		RenderTexture& GetRenderTexture() { return *renderTexture; }
		Window& GetWindow() { return *window; }

	private:

		RendererStats stats;

		RenderTexture* renderTexture = nullptr;
		Window* window = nullptr;
	};
}
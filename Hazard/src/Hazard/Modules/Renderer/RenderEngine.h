#pragma once
#include "Hazard/Core/Core.h"
#include "Platform/System/Window.h"
#include "Hazard/Modules/Module.h"
#include "RendererAPI.h"
#include "Grid.h"
#include "Renderer2D.h"
#include "Hazard/Modules/Scene/Scene.h"
#include "glm/glm.hpp"

namespace Hazard {

	struct RendererStats {
		uint32_t draws;
		uint32_t width;
		uint32_t height;
	};

	class HAZARD_API RenderEngine : public Module {
		friend class Renderer2D;
	public:
		RenderEngine();
		~RenderEngine();

		bool OnEnabled() override;
		void OnEvent(Event& e);
		void Render() override;
		void SceneRender();
		bool OnResized(Event& e);


		Window& GetWindow() { return *window; }
		RendererAPI& GetAPI() { return *api; };
		RendererStats& GetStats() { return *stats; }
		
		RenderTexture* GetRenderTexture() { return renderTexture; }

		static std::string GetError();
		static void Draw(VertexArray* array);
		static RenderEngine* Instance;

	private:
		RendererStats* stats;
		RenderTexture* renderTexture = nullptr;
		std::unique_ptr<Window> window;

		RendererAPI* api = nullptr;
		Renderer2D renderer2D;

		CameraComponent* sceneCamera;
	private:
		static uint32_t boundShader;
	};
}

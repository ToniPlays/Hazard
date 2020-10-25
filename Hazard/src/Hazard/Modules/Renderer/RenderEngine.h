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
	class HAZARD_API RenderEngine : public Module {

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
		
		RenderTexture* GetRenderTexture() { return renderTexture; }

		static std::string GetError();
		static void Draw(VertexArray* array);
		static RenderEngine* Instance;
	private:

		RenderTexture* renderTexture = nullptr;
		std::unique_ptr<Window> window;
		RendererAPI* api = nullptr;

		Renderer2D renderer2D;
	};
}

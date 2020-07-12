#pragma once
#include "Hazard/Core/Core.h"
#include "Platform/System/Window.h"
#include "Renderer3D/Renderer3D.h"
#include "Renderer2D/Renderer2D.h"
#include "Hazard/Modules/Module.h"
#include "RendererAPI.h"
#include "Grid.h"

#include "glm/glm.hpp"

namespace Hazard {
	class HAZARD_API GlobalRenderer : public Module {

	public:
		GlobalRenderer();
		~GlobalRenderer();

		bool OnEnabled() override;
		void OnEvent(Event& e);
		void Render() override;
		void SceneRender();
		bool OnResized(Event& e);

		Window& GetWindow() { return *window; }
		RendererAPI& GetAPI() { return *api; };

		glm::mat4& Get3DProjection() { return renderer3D->GetProjection(); };
		
		RenderTexture* GetRenderTexture() { return renderTexture; }

		static GlobalRenderer* Instance;
		static void Draw(VertexArray* mesh, RenderType type = RenderType::None);
		static float speed;

	private:

		RenderTexture* renderTexture = nullptr;
		std::unique_ptr<Window> window;

		RendererAPI* api = nullptr;
		Renderer3D* renderer3D = nullptr;
		Renderer2D* renderer2D = nullptr;
		Grid* grid;

		int drawCalls = 0;
	public:
		Renderer2D* Get2D();
	};
}

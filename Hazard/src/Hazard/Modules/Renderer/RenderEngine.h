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
		uint32_t draws = 0;
		uint32_t width = 0;
		uint32_t height = 0;

		uint32_t quads = 0;
		uint32_t indices = 0;
		uint32_t batches = 0;

		float GetAspectRatio() {
			if (height == 0) return 0;
			return float(width) / float(height); 
		}

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
		void OnViewResized(uint32_t width, uint32_t height);
		void SetSceneCamera(CameraComponent* camera);


		Window& GetWindow() { return *window; }
		RendererAPI& GetAPI() { return *api; };
		static RendererStats& GetStats() { return Instance->stats; }
		
		RenderTexture* GetRenderTexture() { return renderTexture; }
		
		static void CheckError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
		static void Draw(VertexArray* array, uint32_t indices);
		static RenderEngine* Instance;
		static void SetActiveShader(Shader* shader) { currentShader = shader; }


	private:
		RendererStats stats;
		RenderTexture* renderTexture = nullptr;
		std::unique_ptr<Window> window;

		RendererAPI* api = nullptr;
		Renderer2D renderer2D;

		CameraComponent* sceneCamera = nullptr;

	private:
		static Shader* currentShader;
	};
}

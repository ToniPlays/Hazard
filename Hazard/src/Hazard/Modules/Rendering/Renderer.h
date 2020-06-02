#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Module.h"
#include "Platform/System/Window.h"
#include "Hazard/ECS/GameObject.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"

#include "Hazard/Modules/Rendering/Buffers/VertexArray.h"
#include "Hazard/Modules/Rendering/Buffers/Buffer.h"
#include "Hazard/Modules/Rendering/Shaders/Shader.h"
#include "Hazard/Modules/Rendering/Textures/Texture2D.h"

#include "RendererAPI.h"

#include "glm/glm.hpp"

namespace Hazard {
	class HAZARD_API Renderer : public Module {

	public:

		static float test;
		static bool useGradient;

		Renderer();
		~Renderer();

		void Render() override;
		static void RenderMesh(Mesh* mesh);
		bool OnEvent(Event& e);

		Window& GetWindow() { return *window; }
		static glm::mat4& GetProjection();
		std::vector<GameObject*> GetAll() { return gameObjects; }
		static Renderer* instance;
		RendererAPI& GetAPI() { return *api; };

	private:
		void RenderMeshAsType(Mesh* mesh);
		bool Resized(Event& e);
		RendererAPI* api;

		std::unique_ptr<Window> window;
		std::unique_ptr<VertexArray> vertexArray;

		static glm::mat4 projection;
		static std::vector<GameObject*> gameObjects;
	};
}
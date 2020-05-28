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

namespace Hazard {
	class HAZARD_API Renderer : public Module {
	public:

		Renderer();
		~Renderer();

		void Render() override;
		bool OnEvent(Event& e);

		Window& GetWindow() { return *window; }
		static Matrix4& GetViewMatrix() { return viewMatrix; }
		static float test; 
		static bool useGradient;

		std::vector<GameObject*> GetAll() { return gameObjects; }

	private:

		static std::vector<GameObject*> gameObjects;
		bool Resized(Event& e);
		std::unique_ptr<Window> window;
		std::unique_ptr<VertexArray> vertexArray;

		static Matrix4 viewMatrix;
	};
}
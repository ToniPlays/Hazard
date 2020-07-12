#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/ECS/GameObject.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"

#include "Hazard/Modules/Renderer/Buffers/VertexArray.h"
#include "Hazard/Modules/Renderer/Buffers/Buffer.h"
#include "Hazard/Modules/Renderer/Shaders/Shader.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"

#include "Hazard/ECS/Components/Mesh.h"

namespace Hazard {
	class HAZARD_API Renderer3D {

	public:

		Renderer3D();
		~Renderer3D();

		void Render();
		void OnResized(Event& e);

		static glm::mat4& GetProjection() { return projection; };
		std::vector<GameObject*> GetAll() { return gameObjects; }

	private:
		std::unique_ptr<VertexArray> vertexArray;

		static glm::mat4 projection;
		static std::vector<GameObject*> gameObjects;
	};
}
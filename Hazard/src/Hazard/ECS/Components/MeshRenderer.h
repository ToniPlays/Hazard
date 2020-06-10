#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"


namespace Hazard {
	class HAZARD_API MeshRenderer : public Component {
	public:
		MeshRenderer(Mesh* mesh, Shader* shader, GameObject* gameObject);
		~MeshRenderer();
		std::unique_ptr<VertexArray> vertexArray;
		void OnRender() override;

	private:
		Texture2D* texture;
		Mesh* mesh;
		Shader* shader;
	};
}
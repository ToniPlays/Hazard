#pragma once
#include "Hazard/Modules/Scene/Scene.h"
#include "RendererAPI.h"
#include "Batch/Vertex.h"
#include <glm/glm.hpp>

namespace Hazard {

	struct BatchData {

		VertexArray* VAO = nullptr;
		VertexBuffer* VBO = nullptr;;
		IndexBuffer* IBO = nullptr;;

		uint32_t indexCount = 0;
		Vertex* vertexBuffer = nullptr;
		Vertex* vertexBufferPtr = nullptr;
		
	};
	class HAZARD_API Renderer2D {

	public:

		Renderer2D() {}
		void Init();

		bool BeginScene(CameraComponent* sceneCamera);
		void BeginBatch();
		void EndBatch();
		void DrawBatch();
		void EndScene();

		void Render(Scene* scene);
		void Resize(float w, float h);
		static void DrawQuad(Vector3<float> position, Vector3<float> size, Color color = Color::FromHex("#FFFFFF"));

	private:
		void RenderEntity(Entity* entity);

		static Renderer2D* instance;
		BatchData* batchData;
		Shader* shader;
		glm::mat4 projection;
	};
}
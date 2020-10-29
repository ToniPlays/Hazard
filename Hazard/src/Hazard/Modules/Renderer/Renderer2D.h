#pragma once
#include "Hazard/Modules/Scene/Scene.h"
#include "RendererAPI.h"
#include "Batch/Vertex.h"
#include <glm/glm.hpp>

namespace Hazard {

	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const short MaxTextureSlots = 16;

	struct BatchData {

		VertexArray* VAO = nullptr;
		VertexBuffer* VBO = nullptr;;
		IndexBuffer* IBO = nullptr;;

		uint32_t indexCount = 0;
		Vertex* vertexBuffer = nullptr;
		Vertex* vertexBufferPtr = nullptr;

		uint8_t textureIndex = 1;
		std::array<Texture*, MaxTextureSlots> textures;
		
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
		static void DrawQuad(Vector3<float> position, Vector3<float> size, Color color = Color::FromHex("#FFFFFF"), const char* texture = "");

	private:
		void RenderEntity(Entity* entity);
		uint8_t GetTextureSlot(const char* texture);
		static Renderer2D* instance;
		BatchData* batchData;
		Shader* shader;
		glm::mat4 projection;
	};
}
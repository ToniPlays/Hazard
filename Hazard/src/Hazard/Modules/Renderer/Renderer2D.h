#pragma once
#include "Hazard/Modules/Scene/Scene.h"
#include "RendererAPI.h"
#include "Batch/Vertex.h"
#include "Hazard/Utils/Maths/Transform.h"


namespace Hazard {

	static const uint32_t MaxQuads = 15000;
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
		void EndScene();

		void Render(Scene* scene);
		void Resize(float w, float h);
		static void DrawQuad(Transform transform, Color color = Color::FromHex("#FFFFFF"), const char* texture = "");
	private:
		void RenderEntity(Entity* entity);

		void BeginBatch();
		void EndBatch();
		void DrawBatch();
		void NextBatch();

		void PushQuad(glm::mat4 transformMatrix, Color color, const char* texture);
		uint8_t GetTextureSlot(const char* texture);
		static Renderer2D* instance;
		BatchData* batchData;
		Shader* shader;
		glm::mat4 projection;
	};
}
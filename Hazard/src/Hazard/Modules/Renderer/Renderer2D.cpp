#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "RenderEngine.h"
#include "Buffers/VertexArray.h"
#include "AssetManager.h"

#include "Hazard/Utils/Timer.h"

namespace Hazard {

	Vector3<float> quadVertexPos[] = { { -0.5f, -0.5f, 0.0f},
								  {  0.5f, -0.5f, 0.0f },
								  {  0.5f,  0.5f, 0.0f },
								  { -0.5f,  0.5f, 0.0f} };
	const Vector2<float> textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	

	Renderer2D* Renderer2D::instance = nullptr;

	Renderer2D::~Renderer2D()
	{
		OnDestroy();
	}

	void Renderer2D::Init()
	{
		if (Renderer2D::instance != nullptr) 
			delete Renderer2D::instance;

		Renderer2D::instance = this;

		uint32_t* indices = new uint32_t[MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		batchData = new BatchData();
		batchData->vertexBuffer = new Vertex[MaxVertices];
		batchData->VAO = RendererAPI::Create<VertexArray>();
		batchData->VBO = RendererAPI::VertexBuffer(MaxVertices * sizeof(Vertex));

		batchData->VBO->SetLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float2, "texCoord" },
			{ ShaderDataType::Float4, "color"    },
			{ ShaderDataType::Float , "texIndex" }
		});
		batchData->VAO->AddBuffer(batchData->VBO);

		batchData->IBO = RendererAPI::Create<IndexBuffer>();
		batchData->IBO->Bind();
		batchData->IBO->SetData(indices, MaxIndices);

		delete[] indices;
		shader = AssetManager::GetAsset<Shader>("res/shaders/canvasShader.glsl");

		batchData->textures[0] = AssetManager::GetAsset<Texture2D>("res/textures/white.png");


		for (uint8_t i = 1; i < MaxTextureSlots; i++) {
			batchData->textures[i] = nullptr;
		}

		int32_t samplers[MaxTextureSlots];
		for (uint32_t i = 0; i < MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		shader->Bind();
		shader->SetUniform("textures", samplers, MaxTextureSlots);
	}

	void Renderer2D::OnDestroy() {
		
	}

	bool Renderer2D::BeginScene(CameraComponent* sceneCamera) {
		if (sceneCamera == nullptr) 
			return false;
		RenderEngine::Instance->SetSceneCamera(sceneCamera);
		return true;

	}
	void Renderer2D::BeginBatch()
	{
		PROFILE_FN();
		batchData->VAO->Bind();
		instance->batchData->indexCount = 0;
		batchData->vertexBufferPtr = batchData->vertexBuffer;
		shader->Bind();
		shader->SetUniform("viewProjection", RenderEngine::Instance->sceneCamera->GetViewProjection());
		PROFILE_FN_END();
	}
	void Renderer2D::EndBatch()
	{
		PROFILE_FN();
		GLsizeiptr size = (uint32_t)((uint8_t*)batchData->vertexBufferPtr - (uint8_t*)batchData->vertexBuffer);
		batchData->VBO->SetData(batchData->vertexBuffer, size);
		RenderEngine::Instance->stats.batches++;

		PROFILE_FN_END();
	}
	void Renderer2D::DrawBatch()
	{
		if (batchData->indexCount == 0) return;
		PROFILE_FN();

		for (int i = 0; i < batchData->textureIndex; i++) {
			batchData->textures[i]->Bind(i);
		}

		RenderEngine::Draw(batchData->VAO, batchData->indexCount);
		PROFILE_FN_END();
	}
	void Renderer2D::NextBatch()
	{
		EndBatch();
		DrawBatch();
		BeginBatch();
	}
	void Renderer2D::EndScene() 
	{
		
	}
	void Renderer2D::Render(Scene* scene)
	{
		PROFILE_FN();
		if (!BeginScene(scene->sceneCamera)) 
			return;
		BeginBatch();
		for (Entity* entity : scene->GetChildEntities()) {
			RenderEntity(entity);	
		}
		EndBatch();
		DrawBatch();
		EndScene();
		PROFILE_FN_END();
	}
	void Renderer2D::DrawQuad(Transform transform, Color color, const char* texture)
	{
		instance->PushQuad(Transform::AsMat4(transform) , color, texture);
	}
	void Renderer2D::Resize(float w, float h)
	{

	}
	void Renderer2D::PushQuad(glm::mat4 transformMatrix, Color& color, const char* texture)
	{
		if (batchData->indexCount >= MaxIndices)
			NextBatch();

		constexpr size_t quadVertexCount = 4;
		float id = GetTextureSlot(texture);

		for (uint32_t i = 0; i < quadVertexCount; i++) {

			batchData->vertexBufferPtr->position = transformMatrix * glm::vec4(quadVertexPos[i].x, quadVertexPos[i].y, quadVertexPos[i].z, 1.0f);
			batchData->vertexBufferPtr->texCoord = textureCoords[i];
			batchData->vertexBufferPtr->color = color;
			batchData->vertexBufferPtr->textureIndex = id;
			batchData->vertexBufferPtr++;
		}
		batchData->indexCount += 6;
		RenderEngine::Instance->stats.quads++;
	}
	void Renderer2D::RenderEntity(Entity* entity) {
		for (Component* component : entity->GetComponents()) {
			component->OnRender();
		}

		for (Entity* entity : entity->GetChildEntities()) {
			RenderEntity(entity);
		}
	}
	uint8_t Renderer2D::GetTextureSlot(const char* texture)
	{
		if ((texture == NULL) || (texture[0] == '\0')) return 0;
		PROFILE_FN();
		for (uint8_t i = 1; i < batchData->textureIndex; i++) {
			if (batchData->textures[i]->GetPath() == texture) {
				PROFILE_FN_END();
				return i;
			}
		}
		uint8_t index = batchData->textureIndex;		
		batchData->textures[index] = AssetManager::GetAsset<Texture2D>(texture);
		batchData->textureIndex++;
		PROFILE_FN_END();
		return index;

	}
}

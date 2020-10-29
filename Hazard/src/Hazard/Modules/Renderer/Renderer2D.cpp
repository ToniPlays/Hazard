#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "RenderEngine.h"
#include "Buffers/VertexArray.h"
#include "AssetManager.h"

namespace Hazard {

	

	Renderer2D* Renderer2D::instance = nullptr;
	Transform transform = Transform();

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

	bool Renderer2D::BeginScene(CameraComponent* sceneCamera) {
		return sceneCamera != nullptr;
	}
	void Renderer2D::BeginBatch()
	{
		PROFILE_FN_NAMED("Hazard::Renderer2D batching")
		instance->batchData->indexCount = 0;
		batchData->vertexBufferPtr = batchData->vertexBuffer;

		shader->SetUniform("viewProjection", RenderEngine::Instance->sceneCamera->GetViewProjection());
		shader->SetUniform("transform", Transform::AsMat4(transform));
	}
	void Renderer2D::EndBatch()
	{
		GLsizeiptr size = (uint32_t)((uint8_t*)batchData->vertexBufferPtr - (uint8_t*)batchData->vertexBuffer);
		batchData->VBO->SetData(batchData->vertexBuffer, size);

		PROFILE_FN_END()
	}
	void Renderer2D::DrawBatch()
	{
		if (batchData->indexCount == 0) return;
		PROFILE_FN()
		batchData->VAO->EnableAll();

		for (int i = 0; i < batchData->textureIndex; i++) {
			batchData->textures[i]->Bind(i);
		}
		RenderEngine::Draw(batchData->VAO, batchData->indexCount);
		PROFILE_FN_END()
	}
	void Renderer2D::EndScene() 
	{
		
	}
	void Renderer2D::Render(Scene* scene)
	{
		PROFILE_FN();
		if (!BeginScene(scene->sceneCamera)) 
			return;

		for (Entity* entity : scene->GetChildEntities()) {
			if (!entity->HasComponent<Transform>()) 
				continue;
			transform = *entity->GetComponent<Transform>();

			BeginBatch();
			RenderEntity(entity);	
			EndBatch();
			DrawBatch();
		}
		EndScene();
		PROFILE_FN_END();
	}
	void Renderer2D::DrawQuad(Vector3<float> position, Vector3<float> size, Color color, const char* texture)
	{
		if (instance->batchData->indexCount >= MaxIndices) {

			instance->EndBatch();
			instance->DrawBatch();
			instance->BeginBatch();
		}

		float id = instance->GetTextureSlot(texture);
		instance->batchData->vertexBufferPtr->position = { position.x, position.y, position.z };
		instance->batchData->vertexBufferPtr->texCoord = { 0, 0 };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr->textureIndex = id;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		instance->batchData->vertexBufferPtr->texCoord = { 1, 0 };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr->textureIndex = id;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		instance->batchData->vertexBufferPtr->texCoord = { 1, 1 };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr->textureIndex = id;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		instance->batchData->vertexBufferPtr->texCoord = { 0, 1 };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr->textureIndex = id;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->indexCount += 6;

		RenderEngine::Instance->stats->quads++;
	}
	void Renderer2D::Resize(float w, float h)
	{

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

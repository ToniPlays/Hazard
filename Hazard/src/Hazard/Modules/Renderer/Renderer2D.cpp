#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "RenderEngine.h"
#include "Buffers/VertexArray.h"

namespace Hazard {

	static const uint32_t MaxQuads = 20000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;

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
			{ ShaderDataType::Float4, "color"    }
		});
		batchData->VAO->AddBuffer(batchData->VBO);

		batchData->IBO = RendererAPI::Create<IndexBuffer>();
		batchData->IBO->Bind();
		batchData->IBO->SetData(indices, MaxIndices);

		delete[] indices;
		shader = RendererAPI::Create<Shader>("res/shaders/canvasShader.glsl");

		Texture2D* texture = RendererAPI::Create<Texture2D>("res/textures/checker.png");
	}

	bool Renderer2D::BeginScene(CameraComponent* sceneCamera) {
		uint32_t program = shader->GetProgram();
		if (RenderEngine::boundShader != program) {
			shader->Bind();
			RenderEngine::boundShader = program;
		}
		return sceneCamera != nullptr;
		PROFILE_FN_END();

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
		RenderEngine::Draw(batchData->VAO, batchData->indexCount);
		PROFILE_FN_END()
	}
	void Renderer2D::EndScene() 
	{
		
	}
	void Renderer2D::Render(Scene* scene)
	{
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
	}
	void Renderer2D::DrawQuad(Vector3<float> position, Vector3<float> size, Color color)
	{
		if (instance->batchData->indexCount >= MaxIndices) {

			instance->EndBatch();
			instance->DrawBatch();
			instance->BeginBatch();
		}
		
		instance->batchData->vertexBufferPtr->position = { position.x, position.y, position.z };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		instance->batchData->vertexBufferPtr->color = color;
		instance->batchData->vertexBufferPtr++;

		instance->batchData->vertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		instance->batchData->vertexBufferPtr->color = color;
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
}

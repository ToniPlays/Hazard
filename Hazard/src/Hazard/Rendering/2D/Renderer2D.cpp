#pragma once

#include <hzrpch.h>
#include "Renderer2D.h"

#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Hazard::Rendering {

	Renderer2D::Renderer2D(RenderContext* context) : context(context)
	{
		
	}
	Renderer2D::~Renderer2D()
	{

	}
	void Renderer2D::Init(uint32_t size)
	{
		data.MaxQuads = size;
		data.MaxVertices = data.MaxQuads * 4;
		data.MaxIndices = data.MaxQuads * 6;

		data.QuadVertexArray = RenderUtils::Create<VertexArray>();
		data.QuadVertexBuffer = RenderUtils::Create<VertexBuffer>((uint32_t)(data.MaxVertices * sizeof(QuadVertex)));

		data.QuadVertexBuffer->SetLayout({ 
			{ ShaderDataType::Float3, "v_position" },
			{ ShaderDataType::Float4, "v_color" },
			{ ShaderDataType::Float2, "v_tex_coords" },
			{ ShaderDataType::Float, "v_tex_index" }
			});

		data.QuadVertexArray->AddBuffer(data.QuadVertexBuffer);
		data.QuadVertexBufferBase = new QuadVertex[data.MaxVertices];
		data.QuadVertexBufferPtr = data.QuadVertexBufferBase;
		
		uint32_t* indices = new uint32_t[data.MaxIndices];

		uint32_t offset = 0;

		for (uint32_t i = 0; i < data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* indexBuffer = RenderUtils::Create<IndexBuffer>();
		indexBuffer->SetData(indices, data.MaxIndices);

		delete[] indices;

		data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		
		int samplers[8];
		for (uint32_t i = 0; i < 8; i++)
			samplers[i] = i;

		data.TextureSlots[0] = static_cast<Texture2D*>(RenderUtils::Find<Texture>("White"));

		data.QuadShader = RenderUtils::Create<Shader>("res/Shaders/standard.glsl");
		data.QuadShader->Bind();
		data.QuadShader->SetUniformIntArray("u_Textures", samplers, 8);


		data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}
	void Renderer2D::SubmitQuad(Quad quad)
	{
		constexpr uint8_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (data.QuadIndexCount >= data.MaxIndices || data.TextureIndex > 8) {
			Flush();
			BeginBatch();
		}
		float textureIndex = 0.0f;
		
		if (quad.texture != nullptr) 
		{
			bool found = false;
			for (int i = 1; i < data.TextureIndex; i++) {
				if (&data.TextureSlots[i] == &quad.texture) {
					found = true;
					textureIndex = float(i);
					break;
				}
			}
			if (!found) {
				textureIndex = float(data.TextureIndex);
				data.TextureSlots[data.TextureIndex] = quad.texture;
				data.TextureIndex++;
			}
		}

		for (uint8_t i = 0; i < quadVertexCount; i++) {
			data.QuadVertexBufferPtr->position = quad.transform * data.QuadVertexPos[i];
			data.QuadVertexBufferPtr->color = quad.color;
			data.QuadVertexBufferPtr->texture = textureCoords[i];
			data.QuadVertexBufferPtr->textureIndex = textureIndex;
			data.QuadVertexBufferPtr++;
		}

		data.QuadIndexCount += 6;
	}
	void Renderer2D::BeginScene(glm::mat4 viewProjection)
	{
		data.QuadShader->SetUniformMat4("viewProjection", viewProjection);
	}
	void Renderer2D::BeginBatch()
	{
		data.QuadIndexCount = 0;
		data.QuadVertexBufferPtr = data.QuadVertexBufferBase;
		data.TextureIndex = 1;
	}
	void Renderer2D::Flush()
	{
		if (data.QuadIndexCount == 0) 
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)data.QuadVertexBufferPtr - (uint8_t*)data.QuadVertexBufferBase);
		data.QuadVertexBuffer->SetData(data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < data.TextureIndex; i++)
			data.TextureSlots[i]->Bind(i);

		context->GetWindow().GetContext()->DrawIndexed(data.QuadVertexArray, data.QuadIndexCount);
	}
	void Renderer2D::Close()
	{
		
	}
}
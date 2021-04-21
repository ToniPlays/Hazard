#pragma once

#include <hzrpch.h>
#include "Renderer2D.h"
#include "../RenderCommand.h"

#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Hazard::Rendering {

	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr uint8_t quadVertexCount = 4;

	Renderer2D::Renderer2D(RenderContext* context) {}
	Renderer2D::~Renderer2D() {}

	void Renderer2D::Init(uint32_t size)
	{
		m_Data.MaxQuads = size;
		m_Data.MaxVertices = m_Data.MaxQuads * 4;
		m_Data.MaxIndices = m_Data.MaxQuads * 6;

		m_Data.QuadVertexArray = RenderUtils::Create<VertexArray>();
		m_Data.QuadVertexBuffer = RenderUtils::Create<VertexBuffer>((uint32_t)(m_Data.MaxVertices * sizeof(QuadVertex)));

		m_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "v_position" },
			{ ShaderDataType::Float4, "v_color" },
			{ ShaderDataType::Float2, "v_tex_coords" },
			{ ShaderDataType::Float, "v_tex_index" }
			});

		m_Data.QuadVertexArray->AddBuffer(m_Data.QuadVertexBuffer);
		m_Data.QuadVertexBufferBase = new QuadVertex[m_Data.MaxVertices];
		m_Data.QuadVertexBufferPtr = m_Data.QuadVertexBufferBase;
		
		uint32_t* indices = new uint32_t[m_Data.MaxIndices];
		uint32_t offset = 0;

		for (uint32_t i = 0; i < m_Data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		IndexBuffer* indexBuffer = RenderUtils::Create<IndexBuffer>();
		indexBuffer->SetData(indices, m_Data.MaxIndices);

		delete[] indices;

		m_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		
		int samplers[8];
		for (uint32_t i = 0; i < 8; i++)
			samplers[i] = i;

		m_Data.TextureSlots[0] = static_cast<Texture2D*>(RenderUtils::Find<Texture>("White"));

		m_Data.QuadShader = RenderUtils::Create<Shader>("res/Shaders/standard.glsl");
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetUniformIntArray("u_Textures", samplers, 8);


		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}
	void Renderer2D::SubmitQuad(Quad quad)
	{
		if (m_Data.QuadIndexCount >= m_Data.MaxIndices || m_Data.TextureIndex > 8) {
			Flush();
			BeginBatch();
		}
		float textureIndex = 0.0f;
		
		if (quad.texture != nullptr) 
		{
			bool found = false;
			for (int i = 1; i < m_Data.TextureIndex; i++) {
				if (&m_Data.TextureSlots[i] == &quad.texture) {
					found = true;
					textureIndex = float(i);
					break;
				}
			}
			if (!found) {
				textureIndex = float(m_Data.TextureIndex);
				m_Data.TextureSlots[m_Data.TextureIndex] = quad.texture;
				m_Data.TextureIndex++;
			}
		}

		for (uint8_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->position = quad.transform * m_Data.QuadVertexPos[i];
			m_Data.QuadVertexBufferPtr->color = quad.color;
			m_Data.QuadVertexBufferPtr->texture = textureCoords[i];
			m_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
			m_Data.QuadVertexBufferPtr++;
		}

		RenderCommand::GetStats().quads++;
		RenderCommand::GetStats().vertices += 4;
		m_Data.QuadIndexCount += 6;
	}
	void Renderer2D::BeginScene(glm::mat4 viewProjection)
	{
		m_Data.QuadShader->Bind();
		m_Data.QuadShader->SetUniformMat4("viewProjection", viewProjection);
	}
	void Renderer2D::BeginBatch()
	{
		m_Data.QuadIndexCount = 0;
		m_Data.QuadVertexBufferPtr = m_Data.QuadVertexBufferBase;
		m_Data.TextureIndex = 1;
	}
	void Renderer2D::Flush()
	{
		m_Data.QuadShader->Bind();

		if (m_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_Data.QuadVertexBufferPtr - (uint8_t*)m_Data.QuadVertexBufferBase);
		m_Data.QuadVertexBuffer->SetData(m_Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < m_Data.TextureIndex; i++)
			m_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(m_Data.QuadVertexArray, m_Data.QuadIndexCount);
	}
}
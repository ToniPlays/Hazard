#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Hazard/Rendering/RenderCommand.h"

#include <glad/glad.h>

namespace Hazard::Rendering
{
	Renderer2D::Renderer2D(RenderEngineCreateInfo* info)
	{
		m_Data.MaxQuadCount = info->MaxQuadCount;
		m_Data.MaxVertices = info->MaxQuadCount * 4;
		m_Data.MaxIndices = info->MaxQuadCount * 6;
		m_Data.Samplers = info->SamplerCount;

		uint32_t* indices = new uint32_t[m_Data.MaxIndices];
		uint32_t offset = 0;

		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		for (size_t i = 0; i < m_Data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		VertexBufferCreateInfo vertexInfo = {};
		vertexInfo.Size = m_Data.MaxVertices;
		vertexInfo.Usage = BufferUsage::DynamicDraw;

		IndexBufferCreateInfo indexBuffer = {};
		indexBuffer.Data = indices;
		indexBuffer.Size = m_Data.MaxIndices;
		indexBuffer.Usage = BufferUsage::StaticDraw;

		m_Data.BufferBase = new Vertex2D[m_Data.MaxVertices];
		m_Data.BufferPtr = m_Data.BufferBase;

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.ShaderPath = "res/Shaders/sources/standard.glsl";
		pipelineSpecs.pVertexBuffer = &vertexInfo;
		pipelineSpecs.pIndexBuffer = &indexBuffer;

		m_Pipeline = Pipeline::Create(pipelineSpecs);

		delete[] indices;
	}
	Renderer2D::~Renderer2D()
	{

	}
	void Renderer2D::Render(const RenderPassData& renderPassData)
	{
		RenderCommand::Submit([=]() {
			BeginWorld();
			BeginBatch();

			m_Pipeline->GetShader()->SetUniformBuffer("Camera", (void*)&renderPassData);

			glm::mat4 tempMat = Math::ToTransformMatrix({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
			Submit({ tempMat, "#EF2E2E" });

			tempMat = Math::ToTransformMatrix({ 2.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
			Submit({ tempMat, "#ED1414" });

			tempMat = Math::ToTransformMatrix({ -2.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
			Submit({ tempMat, "#EF2E2E" });


			Flush();
			});
	}
	void Renderer2D::Submit(Quad quad)
	{
		if (m_Data.QuadIndexCount >= m_Data.MaxIndices)
		{
			Flush();
			BeginBatch();
		}
		for (uint8_t i = 0; i < quadVertexCount; i++)
		{
			m_Data.BufferPtr->Position = quad.Transform * m_Data.QuadVertexPos[i];
			m_Data.BufferPtr->Color = quad.Color;
			m_Data.BufferPtr++;
		}
		m_Data.QuadIndexCount += 6;
	}
	void Renderer2D::BeginWorld()
	{
		m_Pipeline->Bind();
	}
	void Renderer2D::BeginBatch()
	{
		m_Data.QuadIndexCount = 0;
		m_Data.BufferPtr = m_Data.BufferBase;
		m_Data.TextureIndex = 1;
	}
	void Renderer2D::Flush()
	{
		if (m_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_Data.BufferPtr - (uint8_t*)m_Data.BufferBase);
		m_Pipeline->GetBuffer()->SetData(m_Data.BufferBase, dataSize);
		m_Pipeline->Bind();
		m_Pipeline->Draw(m_Data.QuadIndexCount);
	}
}
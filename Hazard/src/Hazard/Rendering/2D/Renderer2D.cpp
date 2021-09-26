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

		BufferLayout layout = {
			{ "v_position",		ShaderDataType::Float3 },
			{ "v_color",		ShaderDataType::Float4 }
		};

		VertexBufferCreateInfo vertexInfo = {};
		vertexInfo.Layout = &layout;
		vertexInfo.Data = nullptr;
		vertexInfo.Size = m_Data.MaxVertices;
		vertexInfo.Usage = BufferUsage::DynamicDraw;

		IndexBufferCreateInfo indexBuffer = {};
		indexBuffer.Data = indices;
		indexBuffer.Size = m_Data.MaxIndices;
		indexBuffer.Usage = BufferUsage::StaticDraw;

		VertexArrayCreateInfo vertexArrayInfo = {};
		vertexArrayInfo.VertexBuffer = &vertexInfo;
		vertexArrayInfo.IndexBuffer = &indexBuffer;

		m_Array = VertexArray::Create(vertexArrayInfo);
		delete[] indices;

		m_Data.BufferBase = new Vertex2D[m_Data.MaxVertices];
		m_Data.BufferPtr = m_Data.BufferBase;

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.ShaderPath = "C:/dev/Hazard/HazardEditor/res/Shaders/sources/standard.glsl";
		m_Pipeline = Pipeline::Create(pipelineSpecs);
	}
	Renderer2D::~Renderer2D()
	{
		delete m_Array;
	}
	void Renderer2D::Update()
	{
		RenderCommand::Submit([=]() {
			BeginWorld();
			BeginBatch();

			Submit({ { 0.6, 0, 0 }, { 1.0f, 1.0f, 0.5f, 1.0f} });
			Submit({ {-0.6, 0, 0 }, { 1.0f, 1.0f, 0.5f, 1.0f} });

			Flush();
			});
	}
	void Renderer2D::Submit(Quad quad)
	{
		if (m_Data.QuadIndexCount >= m_Data.MaxIndices) {
			Flush();
			BeginBatch();
		}
		for (uint8_t i = 0; i < quadVertexCount; i++) {
			m_Data.BufferPtr->Position = glm::vec4(quad.Position, 1.0f) + m_Data.QuadVertexPos[i];
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
		if (m_Data.QuadIndexCount == 0) return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_Data.BufferPtr - (uint8_t*)m_Data.BufferBase);
		//m_Array->GetVertexBuffer().SetData(m_Data.BufferBase, dataSize);
		RenderContextCommand::SetClearColor({0.0f, 0.0f, (1.0f + (float)Math::Sin(Time::s_Time)) * 0.5f, 1.0f});
		//RenderCommand::DrawIndexed(m_Array, m_Data.QuadIndexCount);
	}
}
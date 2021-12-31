#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <hzrpch.h>
#include "Renderer2D.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Rendering
{
	Renderer2D::Renderer2D(RenderEngineCreateInfo* info, Ref<RenderCommandBuffer> buffer)
	{
		m_Data.MaxQuadCount = info->MaxQuadCount;
		m_Data.MaxVertices = info->MaxQuadCount * 4;
		m_Data.MaxIndices = info->MaxQuadCount * 6;
		m_Data.Samplers = info->SamplerCount;

		m_Data.TextureSlots.resize(info->SamplerCount);
		m_RenderCommandBuffer = buffer;

		for (uint32_t i = 0; i < m_Data.Samplers; i++) {
			m_Data.TextureSlots[i] = RenderCommand::GetWhiteTexture();
		}
	}
	Renderer2D::~Renderer2D()
	{
		m_Pipeline.Reset();
		m_RenderCommandBuffer.Reset();
	}
	void Renderer2D::Submit(Quad quad)
	{
		if (!(m_CurrentFlags & WorldRenderFlags_::Geometry))
			return;

		if (m_QuadBatch.GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginBatch();
		}

		float textureIndex = FindTexture(quad.Texture.Raw());

		for (uint8_t i = 0; i < quadVertexCount; i++)
		{
			Vertex2D vertex = {};
			vertex.Position = quad.Transform * m_Data.QuadVertexPos[i];
			vertex.Color = quad.Color;
			vertex.TextureCoords = textureCoords[i];
			vertex.TextureIndex = textureIndex;

			m_QuadBatch.Push(vertex);
		}
		m_QuadBatch.AddIndices(6);
	}
	void Renderer2D::SetTargetRenderPass(Ref<RenderPass> renderPass)
	{
		if (!m_Pipeline) {
			Recreate(renderPass);
			return;
		}
		if (m_Pipeline->GetSpecifications().Pass != renderPass) {
			PipelineSpecification specs = m_Pipeline->GetSpecifications();
			specs.Pass = renderPass;
			m_Pipeline = Pipeline::Create(specs);
		}
	}
	void Renderer2D::BeginWorld(const RenderPassData& passData, WorldRenderFlags_ flags)
	{
		m_CurrentFlags = flags;
		m_Pipeline->GetShader()->SetUniformBuffer("Camera", (void*)&passData);
		BeginBatch();
	}
	void Renderer2D::BeginBatch()
	{
		m_Data.TextureIndex = 1.0f;
		m_QuadBatch.Reset();
	}
	void Renderer2D::Flush()
	{
		HZ_SCOPE_PERF("Renderer2D::Flush");
		if (!m_QuadBatch)
			return;

		m_Pipeline->Bind(m_RenderCommandBuffer);
		Ref<Shader> shader = m_Pipeline->GetShader();

		for (uint32_t i = 0; i < m_Data.TextureIndex; i++)
		{
			m_Data.TextureSlots[i]->Bind(i);
			shader->Set("u_Textures", i, m_Data.TextureSlots[i]);
		}

		m_VertexBuffer->SetData(m_QuadBatch.GetData(), m_QuadBatch.GetDataSize());
		m_VertexBuffer->Bind(m_RenderCommandBuffer);
		m_IndexBuffer->Bind(m_RenderCommandBuffer);
		m_Pipeline->Draw(m_RenderCommandBuffer, m_QuadBatch.GetIndexCount());

	}
	void Renderer2D::EndWorld()
	{
		Flush();
	}
	void Renderer2D::Recreate(Ref<RenderPass> renderPass)
	{
		uint32_t offset = 0;

		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		uint32_t* indices = new uint32_t[m_Data.MaxIndices];
		for (size_t i = 0; i < m_Data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		{
			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.DrawType = DrawType::Fill;
			pipelineSpecs.ShaderPath = "Shaders/2D/standard.glsl";
			pipelineSpecs.Pass = std::move(renderPass);

			m_Pipeline = Pipeline::Create(&pipelineSpecs);

			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.Size = m_Data.MaxVertices * sizeof(Vertex2D);
			vertexInfo.Usage = BufferUsage::DynamicDraw;

			m_VertexBuffer = VertexBuffer::Create(&vertexInfo);

			IndexBufferCreateInfo indexBuffer = {};
			indexBuffer.Data = indices;
			indexBuffer.Size = m_Data.MaxIndices;
			indexBuffer.Usage = BufferUsage::StaticDraw;

			m_IndexBuffer = IndexBuffer::Create(&indexBuffer);


			Ref<Shader> shader = m_Pipeline->GetShader();
			shader->Bind();

			for (uint32_t i = 0; i < m_Data.Samplers; i++) {
				shader->Set("u_Textures", i, m_Data.TextureSlots[i]);
			}
		}

		m_QuadBatch = Batch<Vertex2D>(m_Data.MaxQuadCount * 4);
		delete[] indices;
	}
	float Renderer2D::FindTexture(const Ref<Texture2D>& texture)
	{
		for (uint32_t i = 0; i < m_Data.TextureIndex; i++) {
			if (m_Data.TextureSlots[i] == texture) {
				return (float)i;
			}
		}
		m_Data.TextureSlots[m_Data.TextureIndex] = texture;
		return (float)m_Data.TextureIndex++;
	}
}

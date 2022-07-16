
#include <hzrpch.h>
#include "QuadRenderer.h"
#include "HRenderer.h"

namespace Hazard
{
	QuadRenderer::QuadRenderer()
	{
		constexpr uint32_t quadCount = 10000;
		m_Data.MaxQuadCount = quadCount;
		m_Data.MaxVertices = quadCount * 4;
		m_Data.MaxIndices = quadCount * 6;

		m_Data.Samplers = 32;
		m_QuadBatch = Batch<QuadVertex>(quadCount);
	}
	void QuadRenderer::BeginScene()
	{
		BeginBatch();
	}
	void QuadRenderer::EndScene()
	{
		HZR_PROFILE_FUNCTION();
		Flush();
	}
	void QuadRenderer::BeginBatch()
	{
		HZR_PROFILE_FUNCTION();
		m_Data.TextureIndex = 1.0f;
		m_QuadBatch.Reset();
	}
	void QuadRenderer::Flush()
	{	
		HZR_PROFILE_FUNCTION();
		if (!m_QuadBatch) return;

		uint32_t size = m_QuadBatch.GetDataSize();
		uint32_t elements = size / sizeof(QuadVertex);
		m_VertexBuffer->SetData(m_QuadBatch.GetData(), size);

		HRenderer::SubmitMesh(m_VertexBuffer, m_IndexBuffer, m_Pipeline, m_QuadBatch.GetIndexCount());
	}
	void QuadRenderer::SubmitQuad(const glm::mat4& transform, glm::vec4 color)
	{
		HZR_PROFILE_FUNCTION();
		if (m_QuadBatch.GetIndexCount() >= m_Data.MaxIndices) {
			Flush();
			BeginScene();
		}
		SubmitQuad(transform, color, 0.0f);
	}
	void QuadRenderer::SubmitQuad(const glm::mat4& transform, glm::vec4 color, float textureID)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			QuadVertex vertex = {};
			vertex.Position = transform * m_Data.QuadVertexPos[i];
			vertex.Color = color;
			vertex.TextureIndex = textureID;

			m_QuadBatch.Push(vertex);
		}
		m_QuadBatch.AddIndices(6);
	}

	void QuadRenderer::CreateResources()
	{
		using namespace HazardRenderer;

		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		BufferLayout layout = { { "a_Position",		ShaderDataType::Float3 },
								{ "a_Color",		ShaderDataType::Float4 },
								{ "a_TextureIndex",	ShaderDataType::Float  }
		};

		if (!m_IndexBuffer) 
		{
			uint32_t offset = 0;

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

			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.DebugName = "Renderer2DQuadBatch";
			vertexInfo.Size = (uint32_t)m_Data.MaxVertices * sizeof(QuadVertex);
			vertexInfo.Usage = BufferUsage::DynamicDraw;
			vertexInfo.Layout = &layout;
			vertexInfo.Data = nullptr;

			m_VertexBuffer = VertexBuffer::Create(&vertexInfo);

			IndexBufferCreateInfo indexBuffer = {};
			indexBuffer.DebugName = "2DQuadIndexBuffer";
			indexBuffer.Data = indices;
			indexBuffer.Size = m_Data.MaxIndices;
			indexBuffer.Usage = BufferUsage::StaticDraw;

			m_IndexBuffer = IndexBuffer::Create(&indexBuffer);
			//delete[] indices;
		}

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.DebugName = "2DQuadBatchPipeline";
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.DrawType = DrawType::Fill;
		pipelineSpecs.ShaderPath = "Shaders/2D/standard.glsl";
		pipelineSpecs.pTargetRenderPass = m_RenderPass.Raw();
		pipelineSpecs.pBufferLayout = &layout;

		m_Pipeline = Pipeline::Create(&pipelineSpecs);
	}
}



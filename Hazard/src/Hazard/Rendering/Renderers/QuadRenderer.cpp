
#include <hzrpch.h>
#include "QuadRenderer.h"
#include "../HRenderer.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	QuadRenderer::QuadRenderer()
	{

	}
	void QuadRenderer::Init()
	{
		HZR_PROFILE_FUNCTION();
		constexpr size_t quadCount = 50000;
		m_Data.MaxQuadCount = quadCount;
		m_Data.MaxVertices = quadCount * 4;
		m_Data.MaxIndices = quadCount * 6;

		m_Data.Samplers = 32;
		if(m_QuadBatch)
			hdelete m_QuadBatch;

		m_QuadBatch = hnew Batch<QuadVertex>(m_Data.MaxVertices);
		m_Data.TextureSlots.resize(m_Data.Samplers);

		Ref<Image2D> whiteTexture = Application::GetModule<RenderContextManager>().GetDefaultResources().WhiteTexture;
		for (uint32_t i = 0; i < m_Data.Samplers; i++)
			m_Data.TextureSlots[i] = whiteTexture;
	}
	void QuadRenderer::BeginScene()
	{
		HZR_PROFILE_FUNCTION();
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
		m_QuadBatch->Reset();
	}
	void QuadRenderer::Flush()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_QuadBatch) return;

		BufferCopyRegion region = {};
		region.Data = m_QuadBatch->GetData();
		region.Size = m_QuadBatch->GetDataSize();
		region.Offset = 0;

		m_VertexBuffer->SetData(region);
		Ref<Shader> shader = m_Pipeline->GetShader();

		for (uint32_t i = 0; i < m_Data.TextureIndex; i++) 
			shader->Set("u_Textures", i, m_Data.TextureSlots[i]);

		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, m_IndexBuffer, m_Pipeline, m_QuadBatch->GetIndexCount());
	}
	void QuadRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2DAsset> texture)
	{
		HZR_PROFILE_FUNCTION();

		if (!IsVisible(transform)) return;

		if (m_QuadBatch->GetIndexCount() >= m_Data.MaxIndices) 
		{
			Flush();
			BeginScene();
		}

		float textureIndex = 0.0f;
		if(texture)
			textureIndex = GetImageIndex(texture->GetSourceImageAsset()->Value.As<Image2D>());

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (uint8_t i = 0; i < 4; i++)
		{
			QuadVertex vertex = {};
			vertex.Position = transform * m_Data.QuadVertexPos[i];
			vertex.Color = color;
			vertex.TextureCoords = textureCoords[i];
			vertex.TextureIndex = textureIndex;

			m_QuadBatch->Push(vertex);
		}
		m_QuadBatch->AddIndices(6);
	}

	void QuadRenderer::SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, Ref<Texture2DAsset> texture)
	{
		HZR_PROFILE_FUNCTION();

		if (!IsVisible(transform)) return;

		if (m_QuadBatch->GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginScene();
		}

		float textureIndex = 0.0f;
		if (texture) textureIndex = GetImageIndex(texture->GetSourceImageAsset()->Value.As<Image2D>());

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float size = 1.0f;

		const glm::vec4 cameraRightVector	= { view[0][0], view[1][0], view[2][0], 0.0f };
		const glm::vec4 cameraUpVector		= { view[0][1], view[1][1], view[2][1], 0.0f };

		for (uint8_t i = 0; i < 4; i++)
		{
			const glm::vec3& pos = m_Data.QuadVertexPos[i];

			QuadVertex vertex = {};
			vertex.Position = transform[3] + size * pos.x * cameraRightVector +
											 size * pos.y * cameraUpVector;

			vertex.Color = color;
			vertex.TextureCoords = textureCoords[i];
			vertex.TextureIndex = textureIndex;

			m_QuadBatch->Push(vertex);
		}
		m_QuadBatch->AddIndices(6);
	}
	bool QuadRenderer::IsVisible(const glm::mat4& transform)
	{
		return true;
	}
	void QuadRenderer::CreateResources(Ref<RenderPass> renderPass)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Data.QuadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		BufferLayout layout = QuadVertex::Layout();

		if (!m_IndexBuffer)
		{
			uint32_t offset = 0;

			uint32_t* indices = hnew uint32_t[m_Data.MaxIndices];

			for (size_t i = 0; i < m_Data.MaxIndices; i += 6) 
			{
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
			indexBuffer.Size = m_Data.MaxIndices * sizeof(uint32_t);
			indexBuffer.Usage = BufferUsage::StaticDraw;

			m_IndexBuffer = IndexBuffer::Create(&indexBuffer);

			hdelete indices;
		}

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.DebugName = "2DQuadBatchPipeline";
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.DrawType = DrawType::Fill;
		pipelineSpecs.ShaderPath = "res/Shaders/2D/standard.glsl";
		pipelineSpecs.pTargetRenderPass = renderPass;
		pipelineSpecs.pBufferLayout = &layout;

		m_Pipeline = Pipeline::Create(&pipelineSpecs);

		m_RenderPass = renderPass;
	}
	float QuadRenderer::GetImageIndex(const Ref<Image2D>& texture)
	{
		if (!texture) return 0.0f;

		for (uint32_t i = 0; i < m_Data.TextureIndex; i++) {
			if (m_Data.TextureSlots[i] == texture) {
				return (float)i;
			}
		}
		m_Data.TextureSlots[(size_t)m_Data.TextureIndex] = texture;
		return m_Data.TextureIndex++;
	}
}



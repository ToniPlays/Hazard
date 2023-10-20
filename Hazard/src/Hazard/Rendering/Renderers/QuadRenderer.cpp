
#include <hzrpch.h>
#include "QuadRenderer.h"
#include "../HRenderer.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	void QuadRenderer::Init()
	{
		HZR_PROFILE_FUNCTION();
		constexpr uint64_t quadCount = 5000;
		m_Data.MaxQuadCount = quadCount;
		m_Data.MaxVertices = quadCount * 4;
		m_Data.MaxIndices = quadCount * 6;
		m_Data.Samplers = 32;

		if (m_QuadBatch)
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

		for (uint32_t i = m_Data.TextureIndex - 1; i < m_Data.TextureSlots.size(); i++)
			m_Data.TextureSlots[i] = m_Data.TextureSlots[0];
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
		if (m_QuadBatch->GetIndexCount() == 0) return;

		BufferCopyRegion region = {};
		region.Data = m_QuadBatch->GetData();
		region.Size = m_QuadBatch->GetDataSize();
		region.Offset = 0;

		m_VertexBuffer->SetData(region);

		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(m_Material->GetPipeline())->Value.As<Pipeline>();
		pipeline->SetRenderPass(m_RenderPass);
	
		Ref<DescriptorSet> set = m_Material->GetDescriptorSet();
		for (uint32_t i = 0; i < m_Data.TextureIndex; i++)
			set->Write(0, i, m_Data.TextureSlots[i], RenderEngine::GetResources().DefaultImageSampler);

		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, m_IndexBuffer, m_Material, m_QuadBatch->GetIndexCount(), 0);
	}

	void QuadRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2DAsset> texture)
	{
		HZR_PROFILE_FUNCTION();

		if (!IsVisible(transform)) return;

		if (m_QuadBatch->GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginBatch();
		}

		float textureIndex = 0.0f;
		if (texture)
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

		const glm::vec4 cameraRightVector = { view[0][0], view[1][0], view[2][0], 0.0f };
		const glm::vec4 cameraUpVector = { view[0][1], view[1][1], view[2][1], 0.0f };

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

		if (!m_IndexBuffer)
		{
			uint32_t offset = 0;

			uint32_t* indices = hnew uint32_t[m_Data.MaxIndices];

			for (uint64_t i = 0; i < m_Data.MaxIndices; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			BufferCreateInfo iboInfo = {};
			iboInfo.Name = "QuadIndexBuffer";
			iboInfo.Data = indices;
			iboInfo.Size = m_Data.MaxIndices * sizeof(uint32_t);
			iboInfo.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT;

			m_IndexBuffer = GPUBuffer::Create(&iboInfo);

			hdelete[] indices;
		}

		BufferCreateInfo vboInfo = {};
		vboInfo.Name = "QuadVBO";
		vboInfo.Data = nullptr;
		vboInfo.Size = m_Data.MaxVertices * sizeof(QuadVertex);
		vboInfo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

		m_VertexBuffer = GPUBuffer::Create(&vboInfo);

		AssetHandle pipelineHandle = ShaderLibrary::GetPipelineAssetHandle("QuadShader");
		m_Material = Ref<Material>::Create(pipelineHandle);

		Ref<Sampler> sampler = RenderEngine::GetResources().DefaultImageSampler;
		Ref<DescriptorSet> set = m_Material->GetDescriptorSet();

		for (uint32_t i = 0; i < m_Data.Samplers; i++)
			set->Write(0, i, m_Data.TextureSlots[i], sampler, true);

		m_RenderPass = renderPass;
	}

	float QuadRenderer::GetImageIndex(const Ref<Image2D>& texture)
	{
		if (!texture) return 0.0f;

		for (uint32_t i = 0; i < m_Data.TextureIndex; i++) 
		{
			if (m_Data.TextureSlots[i] == texture) 
			{
				return (float)i;
			}
		}
		m_Data.TextureSlots[(uint32_t)m_Data.TextureIndex] = texture;
		return m_Data.TextureIndex++;
	}
}

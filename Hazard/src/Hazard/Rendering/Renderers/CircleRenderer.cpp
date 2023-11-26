
#include <hzrpch.h>
#include "CircleRenderer.h"
#include "../HRenderer.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	void CircleRenderer::Init()
	{
		HZR_PROFILE_FUNCTION();
		constexpr uint64_t circleCount = 5000;
		m_Data.MaxCircleCount = circleCount;
		m_Data.MaxVertices = circleCount * 4;
		m_Data.MaxIndices = circleCount * 6;

		if (m_CicleBatch)
			hdelete m_CicleBatch;

		m_CicleBatch = hnew Batch<CircleVertex>(m_Data.MaxVertices);
	}

	void CircleRenderer::BeginScene()
	{
		HZR_PROFILE_FUNCTION();
		BeginBatch();
	}

	void CircleRenderer::EndScene()
	{
		HZR_PROFILE_FUNCTION();
		Flush();
	}

	void CircleRenderer::BeginBatch()
	{
		HZR_PROFILE_FUNCTION();
		m_CicleBatch->Reset();
	}

	void CircleRenderer::Flush()
	{
		HZR_PROFILE_FUNCTION();
		if (m_CicleBatch->GetIndexCount() == 0) return;

		BufferCopyRegion region = {};
		region.Data = m_CicleBatch->GetData();
		region.Size = m_CicleBatch->GetDataSize();
		region.Offset = 0;

		m_VertexBuffer->SetData(region);

		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(m_Material->GetPipeline())->Value.As<Pipeline>();
		pipeline->SetRenderPass(m_RenderPass);

		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, m_IndexBuffer, m_Material, m_CicleBatch->GetIndexCount(), 0);
	}

	void CircleRenderer::SubmitCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		HZR_PROFILE_FUNCTION();

		if (!IsVisible(transform)) return;

		if (m_CicleBatch->GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginBatch();
		}

		for (uint8_t i = 0; i < 4; i++)
		{
			CircleVertex vertex = {};
			vertex.Position = transform * m_Data.CircleVertexPos[i];
			vertex.LocalPosition = m_Data.CircleVertexPos[i] * 2.0f;
			vertex.Color = color;
			vertex.Thickness = thickness;
			vertex.Fade = fade;

			m_CicleBatch->Push(vertex);
		}
		m_CicleBatch->AddIndices(6);
	}

	void CircleRenderer::SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, float thickness, float fade)
	{
		HZR_PROFILE_FUNCTION();

		if (!IsVisible(transform)) return;

		if (m_CicleBatch->GetIndexCount() >= m_Data.MaxIndices)
		{
			Flush();
			BeginScene();
		}

		//constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float size = 1.0f;

		const glm::vec4 cameraRightVector = { view[0][0], view[1][0], view[2][0], 0.0f };
		const glm::vec4 cameraUpVector = { view[0][1], view[1][1], view[2][1], 0.0f };

		for (uint8_t i = 0; i < 4; i++)
		{
			const glm::vec3& pos = m_Data.CircleVertexPos[i];

			CircleVertex vertex = {};
			vertex.Position = transform[3] + size * pos.x * cameraRightVector +
				size * pos.y * cameraUpVector;

			vertex.LocalPosition = m_Data.CircleVertexPos[i];
			vertex.Color = color;
			vertex.Thickness = thickness;
			vertex.Fade = fade;

			m_CicleBatch->Push(vertex);
		}
		m_CicleBatch->AddIndices(6);
	}

	bool CircleRenderer::IsVisible(const glm::mat4& transform)
	{
		return true;
	}

	void CircleRenderer::SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass)
	{
		if (renderPass == m_RenderPass) return;

		m_RenderPass = renderPass;
		CreateResources(renderPass);
	};

	void CircleRenderer::CreateResources(Ref<RenderPass> renderPass)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Data.CircleVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.CircleVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.CircleVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.CircleVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

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
			iboInfo.Name = "CircleIndexBuffer";
			iboInfo.Data = indices;
			iboInfo.Size = m_Data.MaxIndices * sizeof(uint32_t);
			iboInfo.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT;

			m_IndexBuffer = GPUBuffer::Create(&iboInfo);

			hdelete[] indices;
		}

		BufferCreateInfo vboInfo = {};
		vboInfo.Name = "CircleVBO";
		vboInfo.Data = nullptr;
		vboInfo.Size = m_Data.MaxVertices * sizeof(CircleVertex);
		vboInfo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

		m_VertexBuffer = GPUBuffer::Create(&vboInfo);

		auto layout = DescriptorSetLayout();

		AssetHandle pipelineHandle = ShaderLibrary::GetPipelineAssetHandle("CircleShader");
		m_Material = Ref<Material>::Create(pipelineHandle, layout);

		m_RenderPass = renderPass;
	}
}

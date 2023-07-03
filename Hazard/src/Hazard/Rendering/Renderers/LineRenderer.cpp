
#include <hzrpch.h>
#include "LineRenderer.h"
#include "../HRenderer.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/ShaderLibrary.h"


namespace Hazard
{
	void LineRenderer::Init()
	{
		HZR_PROFILE_FUNCTION();
		constexpr size_t lineCount = 5000;
		m_Data.MaxLineCount = lineCount;
		m_Data.MaxVertices = lineCount * 2;

		if (m_LineBatch)
			hdelete m_LineBatch;
		m_LineBatch = hnew Batch<LineVertex>(m_Data.MaxVertices);
	}
	void LineRenderer::BeginScene()
	{
		HZR_PROFILE_FUNCTION();
		BeginBatch();
	}
	void LineRenderer::EndScene()
	{
		HZR_PROFILE_FUNCTION();
		Flush();
	}
	void LineRenderer::BeginBatch()
	{
		HZR_PROFILE_FUNCTION();
		m_LineBatch->Reset();
	}
	void LineRenderer::Flush()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_LineBatch->GetCount()) return;

		BufferCopyRegion region = {};
		region.Data = m_LineBatch->GetData();
		region.Size = m_LineBatch->GetDataSize();
		region.Offset = 0;

		m_VertexBuffer->SetData(region);

		Ref<AssetPointer> pipeline = AssetManager::GetAsset<AssetPointer>(m_PipelineHandle);

		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, pipeline->Value.As<Pipeline>(), m_LineBatch->GetCount());
	}
	void LineRenderer::SubmitLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec4& color)
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		if (m_LineBatch->GetCount() >= m_Data.MaxVertices)
		{
			Flush();
			BeginBatch();
		}

		LineVertex startVertex = {};
		startVertex.Position = startPos;
		startVertex.Color = color;

		m_LineBatch->Push(startVertex);

		LineVertex endVertex = {};
		endVertex.Position = endPos;
		endVertex.Color = color;

		m_LineBatch->Push(endVertex);
	}

	void LineRenderer::SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass)
	{
		if (renderPass == m_RenderPass)
			return;

		m_RenderPass = renderPass;
		CreateResources(renderPass);
	}

	void LineRenderer::CreateResources(Ref<RenderPass> renderPass)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		BufferLayout layout = { { "a_Position",		ShaderDataType::Float3 },
								{ "a_Color",		ShaderDataType::Float4 }
		};

		BufferCreateInfo vertexBufferInfo = {};
		vertexBufferInfo.Name = "LineRendererVertexBuffer";
		vertexBufferInfo.Size = m_Data.MaxVertices * sizeof(LineVertex);
		vertexBufferInfo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

		m_VertexBuffer = GPUBuffer::Create(&vertexBufferInfo);

		m_PipelineHandle = AssetManager::GetHandleFromKey("LineShader.glsl.hpack");
		m_RenderPass = renderPass;
	}
}



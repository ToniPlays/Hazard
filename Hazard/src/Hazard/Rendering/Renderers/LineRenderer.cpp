
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
		constexpr uint64_t lineCount = 5000;
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

		BufferCopyRegion region = {
			.Size = m_LineBatch->GetDataSize(),
			.Data = m_LineBatch->GetData(),
		};

		m_VertexBuffer->SetData(region);

		Ref<Pipeline> pipeline = ShaderLibrary::GetPipeline("LineShader");
		HRenderer::SubmitMesh(glm::mat4(1.0f), m_VertexBuffer, m_Material, m_LineBatch->GetCount());
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

		LineVertex startVertex = {
			.Position = startPos,
			.Color = color,
		};

		LineVertex endVertex = {
			.Position = endPos,
			.Color = color,
		};

		m_LineBatch->Push(startVertex);
		m_LineBatch->Push(endVertex);
	}

	void LineRenderer::CreateResources()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		BufferCreateInfo vertexBufferInfo = {
			.Name = "LineRendererVertexBuffer",
			.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
			.Size = m_Data.MaxVertices * sizeof(LineVertex),
		};

		m_Material = Ref<Material>::Create(ShaderLibrary::GetPipeline("LineShader"));
		m_VertexBuffer = GPUBuffer::Create(&vertexBufferInfo);
	}
}



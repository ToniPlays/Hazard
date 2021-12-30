#include <hzrpch.h>
#include "DebugRenderer.h"

namespace Hazard::Rendering
{
	DebugRenderer::DebugRenderer(uint32_t lines, Ref<RenderCommandBuffer> cmdBuffer)
	{
		m_RenderData.MaxLineCount = lines;
		m_RenderData.MaxVertices = lines * 2;

		m_CommandBuffer = cmdBuffer;
	}
	DebugRenderer::~DebugRenderer()
	{

	}
	void DebugRenderer::SetTargetRenderPass(Ref<RenderPass> renderPass)
	{
		if (!m_LinePipeline) {
			Recreate(renderPass);
			return;
		}

		if (m_LinePipeline->GetSpecifications().Pass != renderPass)
		{
			PipelineSpecification lineSpec = m_LinePipeline->GetSpecifications();
			lineSpec.Pass = renderPass;
			m_LinePipeline = Pipeline::Create(lineSpec);

			PipelineSpecification circleSpec = m_CirclePipeline->GetSpecifications();
			circleSpec.Pass = renderPass;
			m_CirclePipeline = Pipeline::Create(circleSpec);
		}
	}
	void DebugRenderer::BeginWorld(const RenderPassData& passData, WorldRenderFlags_ flags)
	{
		m_CurrentFlags = flags;
		m_LinePipeline->GetShader()->SetUniformBuffer("Camera", (void*)&passData);
		m_CirclePipeline->GetShader()->SetUniformBuffer("Camera", (void*)&passData);
		BeginLineBatch();
		BeginCircleBatch();
	}
	void DebugRenderer::BeginLineBatch()
	{
		m_LineBatch.Reset();
	}
	void DebugRenderer::EndWorld()
	{
		FlushLines();
		FlushCircles();
	}
	void DebugRenderer::FlushLines()
	{
		if (m_LineBatch.GetCount() == 0)
			return;

		m_LinePipeline->Bind(m_CommandBuffer);
		m_LineVertexBuffer->SetData(m_LineBatch.GetData(), m_LineBatch.GetDataSize());
		m_LineVertexBuffer->Bind(m_CommandBuffer);

		m_LinePipeline->DrawArrays(m_CommandBuffer, m_LineBatch.GetCount());
	}
	void DebugRenderer::SubmitLine(Line line)
	{
		if (!(m_CurrentFlags & WorldRenderFlags_::Lines))
			return;

		if (m_LineBatch.GetCount() >= m_RenderData.MaxVertices) {
			FlushLines();
			BeginLineBatch();
		}

		LineVertex startVertex = {};
		startVertex.Position = line.Start;
		startVertex.Color = line.Color;

		m_LineBatch.Push(startVertex);

		LineVertex endVertex = {};
		endVertex.Position = line.End;
		endVertex.Color = line.Color;

		m_LineBatch.Push(endVertex);
	}
	void DebugRenderer::BeginCircleBatch()
	{
		m_CircleBatch.Reset();
	}
	void DebugRenderer::FlushCircles()
	{
		if (!m_CircleBatch)
			return;

		m_CirclePipeline->Bind(m_CommandBuffer);
		m_CircleVertexBuffer->SetData(m_CircleBatch.GetData(), m_CircleBatch.GetDataSize());
		m_CircleVertexBuffer->Bind(m_CommandBuffer);
		m_CircleIndexBuffer->Bind(m_CommandBuffer);

		m_CirclePipeline->Draw(m_CommandBuffer, m_CircleBatch.GetIndexCount());
	}
	void DebugRenderer::SubmitCircle(Circle circle)
	{
		if (!(m_CurrentFlags & WorldRenderFlags_::Lines))
			return;

		if (m_CircleBatch.GetIndexCount() >= 1500 * 6) {
			FlushCircles();
			BeginCircleBatch();
		}

		for (uint8_t i = 0; i < 4; i++)
		{
			CircleVertex2D vertex = {};

			vertex.Position = circle.Transform * m_QuadPos[i];
			vertex.LocalPosition = m_QuadPos[i] * 2.0f;
			vertex.Color = circle.Color;
			vertex.Thickness = circle.Thickness;
			vertex.Fade = circle.Fade;

			m_CircleBatch.Push(vertex);
		}
		m_CircleBatch.AddIndices(6);
	}
	void DebugRenderer::Recreate(Ref<RenderPass> renderPass)
	{
		//Lines
		{
			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.LineWidth = m_RenderData.LineWidth;
			pipelineSpecs.DrawType = DrawType::Line;
			pipelineSpecs.ShaderPath = "Shaders/lineShader.glsl";
			pipelineSpecs.Pass = renderPass;

			m_LinePipeline = Pipeline::Create(pipelineSpecs);

			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.Size = m_RenderData.MaxLineCount * sizeof(LineVertex);
			vertexInfo.Usage = BufferUsage::DynamicDraw;
			vertexInfo.InputStage = m_LinePipeline->GetShader()->GetShaderData().Stages.at(ShaderType::Vertex);

			m_LineVertexBuffer = VertexBuffer::Create(&vertexInfo);
			m_LineBatch = Batch<LineVertex>(m_RenderData.MaxVertices);
		}
		{
			uint32_t offset = 0;
			uint32_t* indices = new uint32_t[1500 * 6];
			for (size_t i = 0; i < 1500 * 6; i += 6) {
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}


			//Circles
			PipelineSpecification pipelineSpecs = {};
			pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
			pipelineSpecs.LineWidth = m_RenderData.LineWidth;
			pipelineSpecs.DrawType = DrawType::Fill;
			pipelineSpecs.ShaderPath = "Shaders/circleShader.glsl";
			pipelineSpecs.Pass = renderPass;

			m_CirclePipeline = Pipeline::Create(pipelineSpecs);

			//TODO: Circle count?
			VertexBufferCreateInfo vertexInfo = {};
			vertexInfo.Size = m_RenderData.MaxLineCount * sizeof(CircleVertex2D);
			vertexInfo.Usage = BufferUsage::DynamicDraw;
			vertexInfo.InputStage = m_LinePipeline->GetShader()->GetShaderData().Stages.at(ShaderType::Vertex);

			m_CircleVertexBuffer = VertexBuffer::Create(&vertexInfo);

			IndexBufferCreateInfo indexBufferInfo = {};
			indexBufferInfo.Size = 1500 * 6;
			indexBufferInfo.Usage = BufferUsage::StaticDraw;
			indexBufferInfo.Data = indices;

			m_CircleIndexBuffer = IndexBuffer::Create(&indexBufferInfo);
			m_CircleBatch = Batch<CircleVertex2D>(1500 * 4);

			delete[] indices;

		}
		m_QuadPos[0] = {-0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_QuadPos[3] = {-0.5f,  0.5f, 0.0f, 1.0f };
	}
}

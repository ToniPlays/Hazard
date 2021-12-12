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

		if (m_LinePipeline->GetSpecifications().RenderPass != renderPass) {
			PipelineSpecification specs = m_LinePipeline->GetSpecifications();
			specs.RenderPass = renderPass;
			m_LinePipeline = Pipeline::Create(specs);
		}
	}
	void DebugRenderer::BeginWorld(const RenderPassData& passData, WorldRenderFlags_ flags)
	{
		m_CurrentFlags = flags;
		m_LinePipeline->GetShader()->GetUniform("Camera").SetData(&passData);
		BeginBatch();
	}
	void DebugRenderer::BeginBatch()
	{
		m_LineBatch.Reset();
	}
	void DebugRenderer::EndWorld()
	{
		Flush();
	}
	void DebugRenderer::Flush()
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
			Flush();
			BeginBatch();
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
	void DebugRenderer::Recreate(Ref<RenderPass> renderPass)
	{
		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.LineWidth = m_RenderData.LineWidth;
		pipelineSpecs.DrawType = DrawType::Line;
		pipelineSpecs.ShaderPath = "Shaders/lineShader.glsl";
		pipelineSpecs.RenderPass = std::move(renderPass);

		m_LinePipeline = Pipeline::Create(pipelineSpecs);
		VertexBufferCreateInfo vertexInfo = {};

		vertexInfo.Size = m_RenderData.MaxLineCount * sizeof(LineVertex);
		vertexInfo.Usage = BufferUsage::DynamicDraw;
		vertexInfo.InputStage = m_LinePipeline->GetShader()->GetShaderData().Stages.at(ShaderType::Vertex);

		m_LineVertexBuffer = VertexBuffer::Create(&vertexInfo);
		m_LineBatch = Batch<LineVertex>(m_RenderData.MaxVertices);

	}
}

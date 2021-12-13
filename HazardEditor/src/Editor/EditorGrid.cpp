#pragma once

#include "EditorGrid.h"

namespace Editor 
{
	Grid::~Grid() {}

	void Grid::Render(const Editor::EditorCamera& camera)
	{
		struct GridData {
			glm::mat4 proj;
			glm::mat4 view;
			glm::vec3 pos;
			float Near;
			float Far;
			float Scale;
			float Nothing;

		} gridData;

		gridData.proj = camera.GetProjection();
		gridData.view = camera.GetView();
		gridData.pos = camera.GetPosition();
		gridData.Near = camera.GetNearClipping();
		gridData.Far = camera.GetFarClipping();
		gridData.Scale = 1.0f;

		m_Pipeline->GetShader()->SetUniformBuffer("Grid", &gridData);
		RenderCommand::DispatchPipelinePostPass(m_Pipeline, 6);
	}
	void Grid::Invalidate(const Ref<RenderPass> renderPass)
	{
		if (m_Pipeline) {
			PipelineSpecification spec = m_Pipeline->GetSpecifications();
			spec.RenderPass = renderPass;
			m_Pipeline = Pipeline::Create(spec);
			return;
		}

		PipelineSpecification specs = {};
		specs.RenderPass = renderPass;
		specs.DrawType = DrawType::Fill;
		specs.ShaderPath = "Shaders/Grid.glsl";
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.Culling = false;
		specs.IsBackground = false;
		m_Pipeline = Pipeline::Create(specs);
	}
}
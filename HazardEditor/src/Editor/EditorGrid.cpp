
#include "EditorGrid.h"

namespace Editor
{
	Grid::~Grid() 
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{	
		struct GridData {
			float Near;
			float Far;
			float Scale;
			float ScaleFade = 0.0f;

		} gridData;

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
			m_Pipeline->SetRenderPass(renderPass);
			return;
		}

		PipelineSpecification specs = {};
		specs.DebugName = "EditorGrid";
		specs.TargetRenderPass = renderPass;
		specs.ShaderPath = "Shaders/Grid.glsl";
		specs.DrawType = DrawType::Fill;
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.CullMode = CullMode::FrontFace;
		specs.IsShared = false;
		specs.DepthTest = true;

		m_Pipeline = Pipeline::Create(&specs);
	}
}

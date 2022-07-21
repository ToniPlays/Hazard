
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"

namespace Editor
{

	using namespace HazardRenderer;
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

		m_Pipeline->GetShader()->SetUniformBuffer("Grid", &gridData, sizeof(GridData));
		HRenderer::SubmitPipeline(m_Pipeline, 6);
	}
	
	void Grid::Invalidate(Ref<RenderPass> renderPass)
	{
		if (m_Pipeline) {
			PipelineSpecification spec = m_Pipeline->GetSpecifications();
			m_Pipeline->SetRenderPass(renderPass);
			return;
		}

		BufferLayout layout = { {} };

		PipelineSpecification specs = {};
		specs.DebugName = "EditorGrid";
		specs.pTargetRenderPass = renderPass.Raw();
		specs.ShaderPath = "Shaders/Grid.glsl";
		specs.DrawType = DrawType::Fill;
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.CullMode = CullMode::FrontFace;
		specs.IsShared = false;
		specs.DepthTest = true;
		specs.pBufferLayout = &layout;

		m_Pipeline = Pipeline::Create(&specs);
	}
	
}

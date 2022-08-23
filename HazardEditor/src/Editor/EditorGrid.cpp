
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"

namespace Editor
{

	using namespace HazardRenderer;
	Grid::~Grid() 
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{	
		if (!m_ShowGrid) return;

		struct GridData {
			float ZNear;
			float ZFar;
			float Scale;
			float ScaleFade = 1.0f;
		} gridData;
		
		gridData.ZNear = camera.GetNearClipping();
		gridData.ZFar = camera.GetFarClipping();
		gridData.Scale = 1.0f;

		//m_Pipeline->GetShader()->SetUniformBuffer("Grid", &gridData, sizeof(GridData));
		HRenderer::SubmitPipeline(m_Pipeline, 6);
	}
	
	void Grid::Invalidate(Ref<RenderPass> renderPass)
	{
		if (m_Pipeline) 
		{
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
		specs.CullMode = CullMode::None;
		specs.IsShared = false;
		specs.pBufferLayout = &layout;

		m_Pipeline = Pipeline::Create(&specs);
	}
	
}

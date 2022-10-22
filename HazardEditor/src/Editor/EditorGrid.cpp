
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"

namespace Editor
{
	struct GridData
	{
		float Scale;
		float ScaleFade = 1.0f;
	};

	using namespace HazardRenderer;
	Grid::~Grid() 
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{	
		if (!m_ShowGrid) return;

		GridData gridData;
		gridData.Scale = 1.0f;
		gridData.ScaleFade = 1.0f;

		m_GridUniformBuffer->SetData(&gridData, sizeof(GridData));
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

		BufferLayout layout = {};

		PipelineSpecification specs = {};
		specs.DebugName = "EditorGrid";
		specs.pTargetRenderPass = renderPass.Raw();
		specs.ShaderPath = "res/Shaders/Grid.glsl";
		specs.DrawType = DrawType::Fill;
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.CullMode = CullMode::None;
		specs.IsShared = false;
		specs.pBufferLayout = &layout;
		specs.DepthTest = true;
		specs.DepthWrite = false;
		specs.DepthOperator = DepthOp::Less;

		m_Pipeline = Pipeline::Create(&specs);

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Grid";
		uboInfo.Set = 1;
		uboInfo.Binding = 1;
		uboInfo.Size = sizeof(GridData);
		uboInfo.Usage = BufferUsage::DynamicDraw;

		m_GridUniformBuffer = UniformBuffer::Create(&uboInfo);

	}
	
}

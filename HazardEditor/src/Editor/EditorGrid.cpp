
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"

namespace Editor
{
	struct GridData
	{
		float Scale;
		float Zoom = 1.0f;
	};

	using namespace HazardRenderer;
	Grid::~Grid() 
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{	
		if (!m_ShowGrid) return;

		constexpr double log = 10.0;
		GridData gridData;
		gridData.Zoom = std::abs(camera.GetPosition().y);
		gridData.Scale = std::pow(log, std::floor(std::log(gridData.Zoom) / std::log(log)));

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
		uboInfo.Set = 2;
		uboInfo.Binding = 1;
		uboInfo.Size = sizeof(GridData);
		uboInfo.Usage = BufferUsage::DynamicDraw;

		m_GridUniformBuffer = UniformBuffer::Create(&uboInfo);

	}
	
}

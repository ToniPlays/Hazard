
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/RenderContext/ShaderAsset.h"

namespace Editor
{
	using namespace HazardRenderer;

	struct GridData
	{
		float Scale;
		float Zoom = 1.0f;
	};

	Grid::~Grid()
	{

	}

	void Grid::Render(const Editor::EditorCamera& camera)
	{
		if (!m_ShowGrid || !m_Pipeline) return;

		constexpr double log = 10.0;
		GridData gridData;
		gridData.Zoom = std::abs(camera.GetPosition().y);
		gridData.Scale = std::pow(log, std::floor(std::log(gridData.Zoom) / std::log(log)));

		BufferCopyRegion region = {};
		region.Data = &gridData;
		region.Size = sizeof(GridData);

		m_GridUniformBuffer->SetData(region);
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
		return;

		auto shader = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Grid.glsl");

        PipelineSpecification specs = {};
        specs.DebugName = "Grid";
        specs.Usage = PipelineUsage::GraphicsBit;
        specs.DepthOperator = DepthOp::Less;
        specs.pTargetRenderPass = renderPass;
		specs.Flags = PIPELINE_DRAW_FILL | PIPELINE_DEPTH_TEST;
        
        m_Pipeline = Pipeline::Create(&specs);

        BufferCreateInfo uboInfo = {};
        uboInfo.Name = "Grid";
        uboInfo.Size = sizeof(GridData);

        m_GridUniformBuffer = GPUBuffer::Create(&uboInfo);
	}
}

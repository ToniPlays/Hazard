
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

		JobPromise promise = AssetManager::GetAssetAsync<AssetPointer>("res/Shaders/Grid.glsl");

		promise.Then([this](JobNode& node) -> size_t {

			/*
			Job* dependency = system->GetJob(job->Dependency);
			auto& shader = *dependency->Value<Ref<ShaderAsset>>();

			PipelineSpecification specs = {};
			specs.DebugName = "Grid";
			specs.DrawType = DrawType::Fill;
			specs.Usage = PipelineUsage::GraphicsBit;
			specs.CullMode = CullMode::None;
			specs.DepthOperator = DepthOp::Less;
			specs.pTargetRenderPass = nullptr;
			specs.ShaderCodeCount = shader->ShaderCode.size();
			specs.pShaderCode = shader->ShaderCode.data();

			m_Pipeline = Pipeline::Create(&specs);

			UniformBufferCreateInfo uboInfo = {};
			uboInfo.Name = "Grid";
			uboInfo.Set = 2;
			uboInfo.Binding = 1;
			uboInfo.Size = sizeof(GridData);
			uboInfo.Usage = BufferUsage::DynamicDraw;

			m_GridUniformBuffer = UniformBuffer::Create(&uboInfo);
			*/
			return 0;

			});
	}
}

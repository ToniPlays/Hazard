
#include "EditorGrid.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/Math/Time.h"
#include "Hazard/Assets/AssetManager.h"

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

		m_GridUniformBuffer->SetData(&gridData, sizeof(GridData));
		HRenderer::SubmitPipeline(m_Pipeline->Value.As<Pipeline>(), 6);
	}
	
	void Grid::Invalidate(Ref<RenderPass> renderPass)
	{
		if (m_Pipeline) 
		{
			auto& pipeline = m_Pipeline->Value.As<Pipeline>();
			PipelineSpecification spec = pipeline->GetSpecifications();
			pipeline->SetRenderPass(renderPass);
			return;
		}

		AssetHandle handle = AssetManager::GetHandleFromFile("res/Shaders/Grid.glsl");
		m_Pipeline = AssetManager::GetAsset<AssetPointer>(handle);

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Grid";
		uboInfo.Set = 2;
		uboInfo.Binding = 1;
		uboInfo.Size = sizeof(GridData);
		uboInfo.Usage = BufferUsage::DynamicDraw;

		m_GridUniformBuffer = UniformBuffer::Create(&uboInfo);
	}
}

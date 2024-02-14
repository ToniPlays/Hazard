#pragma once

#include "Renderer/RenderGraph.h"

namespace Hazard
{
	using namespace HazardRenderer;

	static Ref<RenderGraph> CreateRasterGraph()
	{
		std::vector<RenderGraphStage> stages;

		auto& geometryStage = stages.emplace_back();
		geometryStage.DebugName = "GeometryPass";
		geometryStage.Enabled = true;
		geometryStage.Stride = 4;
		geometryStage.Execute.Add([](const RenderGraphFuncData& data) {
			
		});

		auto& skyboxPass = stages.emplace_back();
		skyboxPass.DebugName = "SkyboxPass";
		skyboxPass.Enabled = true;
		skyboxPass.Stride = sizeof(EnvironmentData);
		skyboxPass.Execute.Add([](const RenderGraphFuncData& data) mutable {

			EnvironmentData& envData = data.Data<EnvironmentData>();
			Ref<RenderCommandBuffer> cmdBuffer = data.CommandBuffer;
			envData.DescriptorSet->Write(0, envData.CameraBuffer, sizeof(CameraData), data.Iteration * sizeof(CameraData));
			envData.DescriptorSet->Write(1, 0, envData.Cubemap, RenderEngine::GetResources().DefaultImageSampler);
			envData.Pipeline->SetRenderPass(data.CurrentRenderPass);

			cmdBuffer->SetPipeline(envData.Pipeline);
			cmdBuffer->SetDescriptorSet(envData.DescriptorSet, 0);
			cmdBuffer->PushConstants(Buffer(&envData.Constants, sizeof(EnvironmentData::SkyboxConstants)), 0, SHADER_STAGE_FRAGMENT_BIT);
			cmdBuffer->Draw(6);
		});

		RenderGraphCreateInfo info = {
			.DebugName = "RasterRenderGraph",
			.StageCount = stages.size(),
			.pStages = stages.data(),
		};

		return RenderGraph::Create(&info);
	}
}
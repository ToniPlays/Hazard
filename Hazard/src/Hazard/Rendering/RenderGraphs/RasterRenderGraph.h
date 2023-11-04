#pragma once

#include "Renderer/RenderGraph.h"

namespace Hazard
{
	static Ref<HazardRenderer::RenderGraph> CreateRasterGraph()
	{
		using namespace HazardRenderer;

		RenderGraphStage geometryStage = {};
		geometryStage.DebugName = "GeometryPass";
		geometryStage.Enabled = true;
		geometryStage.DataSize = sizeof(GeometryMesh);

		geometryStage.Execute = [](Ref<RenderCommandBuffer> cmdBuffer, void* data) {
			GeometryMesh& mesh = *(GeometryMesh*)data;

			cmdBuffer->SetPipeline(mesh.Pipeline);
			cmdBuffer->SetDescriptorSet(RenderEngine::GetResources().WorldDescriptor, 0);
			if (mesh.MaterialDescriptorSet)
				cmdBuffer->SetDescriptorSet(mesh.MaterialDescriptorSet, 1);
			cmdBuffer->SetVertexBuffer(mesh.VertexBuffer);
			cmdBuffer->Draw(mesh.Count, mesh.IndexBuffer);
		};
		RenderGraphStage skyboxStage = {};
		skyboxStage.DebugName = "SkyboxPass";
		skyboxStage.Enabled = true;
		skyboxStage.DataSize = sizeof(EnvironmentData);
		skyboxStage.Execute = [](Ref<RenderCommandBuffer> cmdBuffer, void* data) {
			EnvironmentData& env = *(EnvironmentData*)data;

			cmdBuffer->SetPipeline(env.Pipeline);
			cmdBuffer->SetDescriptorSet(env.MaterialDescriptorSet, 0);
			cmdBuffer->Draw(6);
		};


		std::vector<RenderGraphStage> stages = {
			skyboxStage, geometryStage
		};

		RenderGraphCreateInfo info = {};
		info.DebugName = "RasterRenderGraph";
		info.StageCount = stages.size();
		info.pStages = stages.data();

		return RenderGraph::Create(&info);
	}
}
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
		geometryStage.OnPrepare = [](Ref<RenderCommandBuffer> cmdBuffer) {
		};
		geometryStage.OnFinished = [](Ref<RenderCommandBuffer> cmdBuffer) {

		};
		geometryStage.Execute = [](Ref<RenderCommandBuffer> cmdBuffer, void* data) {
			GeometryMesh& mesh = *(GeometryMesh*)data;

			cmdBuffer->SetPipeline(mesh.Pipeline);
			cmdBuffer->SetDescriptorSet(RenderEngine::GetResources().WorldDescriptor, 0);
			if (mesh.MaterialDescriptorSet)
				cmdBuffer->SetDescriptorSet(mesh.MaterialDescriptorSet, 1);
			cmdBuffer->SetVertexBuffer(mesh.VertexBuffer);
			cmdBuffer->Draw(mesh.Count, mesh.IndexBuffer);
		};
		geometryStage.OnDisabled = [](Ref<RenderCommandBuffer> cmdBuffer) {

		};

		std::vector<RenderGraphStage> stages = {
			geometryStage
		};

		RenderGraphCreateInfo info = {};
		info.DebugName = "RasterRenderGraph";
		info.StageCount = stages.size();
		info.pStages = stages.data();

		return RenderGraph::Create(&info);
	}
}
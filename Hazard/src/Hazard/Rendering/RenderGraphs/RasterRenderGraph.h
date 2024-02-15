#pragma once

#include "Renderer/RenderGraph.h"

namespace Hazard
{
	using namespace HazardRenderer;

	static Ref<RenderGraph> CreateRasterGraph(RenderEngine* engine)
	{
		std::vector<RenderGraphStage> stages;

		auto& geometryStage = stages.emplace_back();
		geometryStage.DebugName = "GeometryPass";
		geometryStage.Enabled = true;
		geometryStage.Stride = sizeof(MeshData);
		geometryStage.Execute.Add([](const RenderGraphFuncData& data) {
			auto& userData = data.Data<GraphBeginData>();
			auto& instances = data.DrawData<MeshData>();

			Ref<RenderCommandBuffer> cmdBuffer = userData.GraphicsBuffer;
			instances.Material->GetPipeline()->SetRenderPass(userData.OutputRenderpass);

			cmdBuffer->SetPipeline(instances.Material->GetPipeline());
			cmdBuffer->SetVertexBuffer(instances.VertexBuffer);
			cmdBuffer->SetDescriptorSet(userData.CameraDescriptor, 0);

			if (instances.Material->GetDescriptorSet())
				cmdBuffer->SetDescriptorSet(instances.Material->GetDescriptorSet(), 1);
			
			Buffer constants = instances.Material->GetPushConstantData();
			if (constants.Data)
				cmdBuffer->PushConstants(constants, 0, SHADER_STAGE_FRAGMENT_BIT);

			cmdBuffer->Draw(instances.Count, instances.IndexBuffer, 0);
		});

		auto& skyboxPass = stages.emplace_back();
		skyboxPass.DebugName = "SkyboxPass";
		skyboxPass.Enabled = true;
		skyboxPass.Stride = sizeof(EnvironmentData);
		skyboxPass.Execute.Add([](const RenderGraphFuncData& data) mutable {

			auto& userData = data.Data<GraphBeginData>();
			auto& envData = data.DrawData<EnvironmentData>();
			if (!envData.Pipeline) return;

			Ref<RenderCommandBuffer> cmdBuffer = userData.GraphicsBuffer;

			envData.Pipeline->SetRenderPass(userData.OutputRenderpass);

			cmdBuffer->SetPipeline(envData.Pipeline);
			cmdBuffer->SetDescriptorSet(userData.CameraDescriptor, 0);
			cmdBuffer->SetDescriptorSet(envData.DescriptorSet, 1);
			cmdBuffer->PushConstants(Buffer(&envData.Constants, sizeof(EnvironmentData::SkyboxConstants)), 0, SHADER_STAGE_FRAGMENT_BIT);
			cmdBuffer->Draw(6);
		});

		RenderGraphCreateInfo info = {
			.DebugName = "RasterRenderGraph",
			.StageCount = stages.size(),
			.pStages = stages.data(),
		};

		info.OnPrepare.Add([engine](const RenderGraphFuncData& data) {
			auto& resources = RenderEngine::GetResources();
			auto& camera = data.Data<GraphBeginData>();

			BufferCopyRegion region = {
				.Size = sizeof(CameraData),
				.Offset = sizeof(CameraData) * data.Iteration,
				.Data = &camera.Camera,
			};

			AssetHandle brdfhandle = AssetManager::AssetHandleFromFile("res/Textures/BRDF_LUT.tga");

			resources.CameraUniformBuffer->SetData(region);
			camera.CameraDescriptor->Write(0, resources.CameraUniformBuffer, sizeof(CameraData), data.Iteration * sizeof(CameraData), false);
			camera.CameraDescriptor->Write(1, 0, resources.WhiteCubemap, resources.DefaultImageSampler, false);
			camera.CameraDescriptor->Write(2, 0, resources.WhiteCubemap, resources.DefaultImageSampler, false);
			camera.CameraDescriptor->Write(3, 0, AssetManager::GetAsset<Texture2DAsset>(brdfhandle)->GetSourceImage(), resources.DefaultImageSampler, false);
		});

		info.OnPrepare.Add([engine](const RenderGraphFuncData& data) mutable {
			auto& userData = data.Data<GraphBeginData>();
			Ref<RenderCommandBuffer> cmdBuffer = userData.GraphicsBuffer;
			cmdBuffer->BeginRenderPass(userData.OutputRenderpass);
		});

		info.OnFinished.Add([](const RenderGraphFuncData& data) mutable {
			Ref<RenderCommandBuffer> cmdBuffer = data.Data<GraphBeginData>().GraphicsBuffer;
			cmdBuffer->EndRenderPass();
		});

		return RenderGraph::Create(&info);
	}
}
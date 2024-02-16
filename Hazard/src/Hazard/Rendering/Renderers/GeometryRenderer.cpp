
#include <hzrpch.h>
#include "GeometryRenderer.h"

namespace Hazard
{
	void GeometryRenderer::Init(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer)
	{
		BufferCreateInfo transformBufferSpec = {
				.Name = "TransformBuffer",
				.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
				.Size = sizeof(MeshTransform) * BIT(16),
		};

		m_TransformBuffer = GPUBuffer::Create(&transformBufferSpec);
		m_CommandBuffer = commandBuffer;
	}

	void GeometryRenderer::Prepare(std::map<MeshKey, MeshData>& geometries, Ref<HazardRenderer::RenderPass> renderPass, Ref<HazardRenderer::DescriptorSet> cameraDescriptor)
	{
		HZR_PROFILE_FUNCTION();
		m_GeometryData = &geometries;
		m_CameraDescriptor = cameraDescriptor;

		ProcessTransforms(geometries);
		PrepareRenderpass(renderPass);
	}

	void GeometryRenderer::Render()
	{
		using namespace HazardRenderer;

		for (auto& [key, data] : *m_GeometryData)
		{
			Ref<GPUBuffer> vertexBuffer = data.VertexBuffer;
			Ref<GPUBuffer> indexBuffer = data.IndexBuffer;
			Ref<Pipeline> pipeline = data.Material->GetPipeline();
			Ref<DescriptorSet> set = data.Material->GetDescriptorSet();
			Buffer constants = data.Material->GetPushConstantData();

			m_CommandBuffer->SetPipeline(pipeline);
			m_CommandBuffer->SetVertexBuffer(vertexBuffer, 0);
			m_CommandBuffer->SetVertexBuffer(m_TransformBuffer, 1, data.TransformOffset);
			m_CommandBuffer->SetDescriptorSet(m_CameraDescriptor, 0);

			if (set)
				m_CommandBuffer->SetDescriptorSet(set, 1);

			if (constants.Size > 0)
				m_CommandBuffer->PushConstants(constants, 0, SHADER_STAGE_FRAGMENT_BIT);
			m_CommandBuffer->DrawInstanced(data.Count, data.Transforms.size(), indexBuffer, 0);
		}
	}

	void GeometryRenderer::ProcessTransforms(std::map<MeshKey, MeshData>& geometries)
	{
		uint32_t offset = 0;
		for (auto& [key, data] : geometries)
		{
			data.TransformOffset = offset;
			BufferCopyRegion region = {
				.Size = data.Transforms.size() * sizeof(MeshTransform),
				.Offset = offset,
				.Data = data.Transforms.data()
			};
			m_TransformBuffer->SetData(region);

			offset += region.Size;
		}
	}
	void GeometryRenderer::PrepareRenderpass(Ref<HazardRenderer::RenderPass> renderPass)
	{
		for (auto& [key, data] : *m_GeometryData)
			data.Material->GetPipeline()->SetRenderPass(renderPass);
	}
}
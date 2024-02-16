#pragma once

#include "Hazard/Rendering/RendererDrawList.h"

namespace Hazard
{
	class GeometryRenderer
	{
	public:
		GeometryRenderer() = default;
		~GeometryRenderer() = default;
		
		void Init(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer);
		void Prepare(std::map<MeshKey, MeshData>& geometries, Ref<HazardRenderer::RenderPass> renderPass, Ref<HazardRenderer::DescriptorSet> cameraDescriptor);
		void Render();

	private:
		void ProcessTransforms(std::map<MeshKey, MeshData>& geometries);
		void PrepareRenderpass(Ref<HazardRenderer::RenderPass> renderPass);

	private:
		Ref<HazardRenderer::GPUBuffer> m_TransformBuffer;
		Ref<HazardRenderer::DescriptorSet> m_CameraDescriptor;
		Ref<HazardRenderer::RenderCommandBuffer> m_CommandBuffer;
		std::map<MeshKey, MeshData>* m_GeometryData;

	};
}
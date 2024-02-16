#pragma once

#include "Hazard/Rendering/RendererDrawList.h"

namespace Hazard
{
	class EnvironmentRenderer
	{
	public:
		EnvironmentRenderer() = default;
		~EnvironmentRenderer() = default;

		void Init(Ref<HazardRenderer::RenderCommandBuffer> commandBuffer);
		void Prepare(EnvironmentData& environment, Ref<RenderPass> renderPass);
		void Render();

	private:
		Ref<HazardRenderer::RenderCommandBuffer> m_CommandBuffer;

		EnvironmentData* m_Environment;
	};
}
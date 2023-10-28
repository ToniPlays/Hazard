#pragma once

#include "Ref.h"
#include "RenderGraphInfo.h"
#include "Core/Rendering/RenderCommandBuffer.h"

#include <atomic>

namespace HazardRenderer
{
	class RenderGraph : public RefCount
	{
	public:
		~RenderGraph() = default;

		void Execute(Ref<RenderCommandBuffer> commandBuffer);
		float GetExecutionTime() { return m_ExecutionTime; }
		static Ref<RenderGraph> Create(RenderGraphCreateInfo* createInfo);

	private:
		RenderGraph(RenderGraphCreateInfo* createInfo);
		void CreateGraphTimeline(RenderGraphCreateInfo* createInfo);

	private:
		std::string m_DebugName;

		std::atomic<float> m_ExecutionTime = 0.0f;
	};
}
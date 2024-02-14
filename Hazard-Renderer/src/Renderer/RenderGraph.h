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

		void Execute(Ref<RenderCommandBuffer> commandBuffer, Ref<RenderPass> outputRenderpass);
		float GetExecutionTime() { return m_ExecutionTime; }

		void SetResource(const std::string& name, void* data, uint64_t size);
		void SetStageActive(const std::string& name, bool enabled);

		void Reset() {
			m_Iterations = 0;
		}

		static Ref<RenderGraph> Create(RenderGraphCreateInfo* createInfo);

	private:
		RenderGraph(RenderGraphCreateInfo* createInfo);

	private:
		std::string m_DebugName;
		std::vector<RenderGraphStage> m_Stages;
		std::unordered_map<std::string, Buffer> m_Resources;

		std::atomic_uint32_t m_Iterations = 0;
		std::atomic<float> m_ExecutionTime = 0.0f;
	};
}
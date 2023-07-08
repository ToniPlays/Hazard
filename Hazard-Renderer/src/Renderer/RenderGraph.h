#pragma once

#include "Ref.h"
#include "RenderGraphInfo.h"
#include "Backend/Core/RenderCommandBuffer.h"

#include <atomic>

namespace HazardRenderer
{
	struct InputResourceData
	{
		InputResource Resource;
		Buffer InputData;
	};

	class RenderGraph : public RefCount
	{
	public:
		~RenderGraph() = default;

		void SetInput(const std::string& input, void* data, uint64_t count);
		void Execute(Ref<RenderCommandBuffer> commandBuffer);

		float GetExecutionTime() { return m_ExecutionTime; }

		static Ref<RenderGraph> Create(RenderGraphCreateInfo* createInfo);

	private:
		RenderGraph(RenderGraphCreateInfo* createInfo);
		void CreateGraphTimeline(RenderGraphCreateInfo* createInfo);
		ResourceReference& GetResourceReference(const RenderGraphStage& stage, uint32_t inputIndex, uint64_t resourceIndex);

	private:
		std::string m_DebugName;
		std::vector<RenderGraphStage> m_Stages;
		std::vector<InputResourceData> m_InputResources;
		std::vector<StageDescriptor> m_StageDescriptors;
		std::unordered_map<std::string, uint32_t> m_InputResourceBindings;

		std::atomic<float> m_ExecutionTime = 0.0f;
	};
}
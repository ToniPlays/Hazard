#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/RenderCommandBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct PipelineStatistic 
	{
		uint64_t InputAssemblyVertices = 0;
		uint64_t InputAssemblyPrimitives = 0;
		uint64_t VertexShaderInvocations = 0;
		uint64_t ClippingInvocations = 0;
		uint64_t ClippingPrimitives = 0;
		uint64_t FragmentShaderInvocations = 0;
		uint64_t ComputeShaderInvocations = 0;
	};

	class VulkanRenderCommandBuffer : public RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(uint32_t size = 0, const std::string& name = "");
		VulkanRenderCommandBuffer(const std::string& name, bool swapchain);
		~VulkanRenderCommandBuffer();

		uint32_t GetFrameIndex() { return m_FrameIndex; };
		
		bool IsRecording() { return m_State == State::Record; };
		void Begin();
		void End();
		void Submit();

		virtual void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false);
		virtual void BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear = false);
		virtual void EndRenderPass();
		virtual void EndRenderPass_RT();

		virtual void BindPipeline(Ref<Pipeline> pipeline) {};
		virtual void SetViewport(float x, float y, float width, float height) {};
		virtual void SetLineSize(float size) {};


		VkCommandBuffer GetBuffer(uint32_t index) { return m_CommandBuffers[index]; }
		VkFence GetFence(uint32_t index) { return m_WaitFences[index]; }

	private:
		void SetState(State state) { m_State = state; }

	private:
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		VkCommandBuffer m_ActiveCommandBuffer = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;
		std::vector<VkQueryPool> m_TimestampQueryPools;
		std::vector<VkQueryPool> m_PipelineQueryPools;
		std::vector<std::vector<uint64_t>> m_TimestampQueryResults;
		std::vector<std::vector<float>> m_GPUExecutionTimes;
		std::vector<std::vector<PipelineStatistic>> m_PipelineStatisticQueryResults;

		uint32_t m_FrameIndex = 0;
		uint32_t m_TimestampQueryCount = 0;
		uint32_t m_PipelineQueryCount = 0;
		uint32_t m_TimestampNextAvailQuery = 2;

		bool m_OwnedBySwapchain = false;

		State m_State = State::Waiting;
	};
}
#endif
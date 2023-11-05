#pragma once
#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Rendering/RenderCommandBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanPipeline;

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

	class VulkanRenderCommandBuffer : public RenderCommandBuffer 
	{
	public:
		VulkanRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t count);
		VulkanRenderCommandBuffer(const std::string& name, bool swapchain);
		~VulkanRenderCommandBuffer();

		uint32_t GetFrameIndex() override { return m_FrameIndex; };
		bool IsRecording() override { return m_State == State::Record; };

		void Begin() override;
		void End() override;
		void Submit() override;

		void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false) override;
		void BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear = false);
		void EndRenderPass() override;

		void SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding) override;
		void SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set) override;
		void PushConstants(Buffer buffer, uint32_t offset, uint32_t flags) override;
		void SetPipeline(Ref<Pipeline> pipeline) override;
		void SetLineWidth(float width) override;

		void Draw(uint64_t count, Ref<GPUBuffer> indexBuffer = nullptr) override;
		void DrawInstanced(uint64_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer = nullptr) override;
		void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset = 0, Ref<GPUBuffer> indexBuffer = nullptr) override;
		void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset = 0, uint32_t maxDraws = 0, Ref<GPUBuffer> indexBuffer = nullptr) override;

		void DispatchCompute(GroupSize globalGroupSize) override;
		void TraceRays(const TraceRaysInfo& traceRaysInfo) override;

		void CopyToBuffer(Ref<GPUBuffer> targetBuffer, const BufferCopyRegion& region);
		void CopyToImage(Ref<Image> targetImage, const ImageCopyRegion& region);
		void BlitImage(const BlitImageInfo& blitInfo);

		void ImageMemoryBarrier(const ImageMemoryInfo& imageMemory);

		//Vulkan specific
		VkCommandBuffer GetBuffer(uint32_t index) { return m_CommandBuffers[index]; }
		VkFence GetFence(uint32_t index) { return m_WaitFences[index]; }

		void GetQueryPoolResults_RT();

	private:
		void BeginPerformanceQuery_RT();
		void EndPerformanceQuery_RT();

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
		DeviceQueue m_DeviceQueue;

		bool m_OwnedBySwapchain = false;

		Ref<VulkanPipeline> m_CurrentPipeline = nullptr;

		State m_State = State::Waiting;
		VkQueue m_SubmitQueue = VK_NULL_HANDLE;

		inline static VkFence s_ComputeFence = VK_NULL_HANDLE;
	};
}
#endif

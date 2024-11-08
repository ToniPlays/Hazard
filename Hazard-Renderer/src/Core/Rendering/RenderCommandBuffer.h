#pragma once

#include "Core/Core.h"
#include "Core/RenderContextCreateInfo.h"

#include "RenderPass.h"
#include "Pipeline.h"
#include "Cubemap.h"
#include "DescriptorSet.h"

namespace HazardRenderer
{
	using GroupSize = glm::uvec3;
	enum class DeviceQueue { GraphicsBit, ComputeBit, TransferBit };

	enum ImageLayout
	{
		IMAGE_LAYOUT_UNDEFINED = 0,
		IMAGE_LAYOUT_GENERAL = BIT(0),
		IMAGE_LAYOUT_COLOR_ATTACHMENT = BIT(1),
		IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT = BIT(2),
		IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY = BIT(3),
		IMAGE_LAYOUT_SHADER_READ_ONLY = BIT(4),
		IMAGE_LAYOUT_TRANSFER_SRC = BIT(5),
		IMAGE_LAYOUT_TRANSFER_DST = BIT(6),
		IMAGE_LAYOUT_DEPTH_READ_ONLY = BIT(7),
		IMAGE_LAYOUT_DEPTH_ATTACHMENT = BIT(8),
	};

	struct DrawIndirectCommand
	{
		uint32_t VertexCount;
		uint32_t InstanceCount;
		uint32_t FirstVertex = 0;
		uint32_t FirstInstance = 0;
	};
	struct DrawIndirectIndexedCommand
	{
		uint32_t IndexCount;
		uint32_t InstanceCount;
		uint32_t FirstIndex = 0;
		uint32_t VertexOffset = 0;
		uint32_t FirstInstance = 0;
	};

	struct BlitImageInfo
	{
		Ref<Image> Image;
		Extent SrcExtent;
		Extent DstExtent;
		uint32_t SrcLayer;
		uint32_t DstLayer;

		uint32_t SrcMip;
		uint32_t DstMip;
		uint32_t SrcLayout;
		uint32_t DstLayout;
	};

	struct ImageMemoryInfo
	{
		Ref<Image> Image;
		uint32_t BaseLayer;
		uint32_t LayerCount;
		uint32_t BaseMip;
		uint32_t MipCount;
		uint32_t SrcLayout;
		uint32_t DstLayout;
	};

	//TODO: Check if needed
	struct TraceRaysInfo
	{
		Extent Extent;
		//Ref<ShaderBindingTable> pBindingTable;
	};

	class RenderCommandBuffer : public RefCount
	{
		friend class Pipeline;
	public:
		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		virtual void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false) = 0;
		virtual void EndRenderPass() = 0;

		//Pipeline resources
		virtual void SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding = 0, uint64_t bufferOffset = 0) = 0;
		virtual void SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set) = 0;
		virtual void PushConstants(Buffer buffer, uint32_t offset, uint32_t flags) = 0;
		virtual void SetPipeline(Ref<Pipeline> pipeline) = 0;
		virtual void SetLineWidth(float width) = 0;

		//Draw
		virtual void Draw(uint64_t count, Ref<GPUBuffer> indexBuffer = nullptr, uint32_t bufferOffset = 0) = 0;
		virtual void DrawInstanced(uint64_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer = nullptr, uint32_t bufferOffset = 0) = 0;
		virtual void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset = 0, Ref<GPUBuffer> indexBuffer = nullptr) = 0;
		virtual void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset = 0, uint32_t maxDraws = 0, Ref<GPUBuffer> indexBuffer = nullptr) = 0;

		//Compute
		virtual void DispatchCompute(GroupSize GlobalGroupSize) = 0;
		virtual void TraceRays(const TraceRaysInfo& traceRaysInfo) = 0;

		//Copy between CPU and GPU
		virtual void CopyToBuffer(Ref<GPUBuffer> destinationBuffer, const BufferCopyRegion& region) = 0;
		virtual void CopyToImage(Ref<Image> destinationImage, const ImageCopyRegion& region) = 0;

		//Copy within GPU
		virtual void CopyBufferToImage(Ref<GPUBuffer> sourceBuffer, Ref<Image2D> destinationImage, const BufferCopyRegion& region) = 0;
		virtual void CopyImageToBuffer(Ref<Image2D> sourceImage, Ref<GPUBuffer> destinationBuffer, const ImageCopyRegion& region) = 0;

		virtual void CopyBufferToBuffer(Ref<GPUBuffer> sourceBuffer, const BufferCopyRegion& sourceRegion, Ref<GPUBuffer> destinationBuffer, const BufferCopyRegion& targetRegion) = 0;

		virtual void BlitImage(const BlitImageInfo& blitInfo) = 0;

		virtual void ImageMemoryBarrier(const ImageMemoryInfo& imageMemory) = 0;

		virtual void OnCompleted(std::function<void()> callback) = 0;

		virtual uint32_t GetFrameIndex() = 0;

	public:
		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", DeviceQueue queue = DeviceQueue::GraphicsBit, uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}

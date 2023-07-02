#pragma once

#include "Core.h"
#include "RenderContextCreateInfo.h"

#include "RenderPass.h"
#include "Pipeline.h"
#include "Texture.h"
#include "DescriptorSet.h"
#include "RTCore/ShaderBindingTable.h"

namespace HazardRenderer 
{
	using GroupSize = glm::uvec3;
	enum class State { Waiting, Record, Finished, Submit };
    enum class DeviceQueue { GraphicsBit, ComputeBit, TransferBit };

	enum MemoryBarrierFlags : uint32_t
	{
		MemoryBarrierBit_All = BIT(0),
		MemoryBarrierBit_Image = BIT(1),
		MemoryBarrierBit_Buffer = BIT(2)
	};

	enum ImageLayout : uint32_t
	{
		ImageLayout_ShaderReadOnly = BIT(0),
		ImageLayout_General = BIT(1)
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

	//Fix this
	struct MemoryBarrierInfo
	{
		MemoryBarrierFlags Flags;
		
		Ref<Image2D> Image = nullptr;
		Ref<CubemapTexture> Cubemap = nullptr;
	};
	struct ImageTransitionInfo
	{
		ImageLayout SourceLayout;
		ImageLayout DestLayout;
		Ref<Image2D> Image = nullptr;
		Ref<CubemapTexture> Cubemap = nullptr;
	};

	struct GenMipmapsInfo
	{
		Ref<Image2D> Image = nullptr;
		Ref<CubemapTexture> Cubemap = nullptr;
	};

	//TODO: Check if needed
	struct TraceRaysInfo
	{
		Extent Extent;
		Ref<ShaderBindingTable> pBindingTable;
	};
	//TODO: Check if needed
	struct AccelerationStructureBuildInfo 
	{
		BuildType Type;
		Ref<AccelerationStructure> AccelerationStructure;
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
		virtual void SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding = 0) = 0;
		virtual void SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set) = 0;
		virtual void PushConstants(Buffer buffer, uint32_t offset, uint32_t flags) = 0;
		virtual void SetPipeline(Ref<Pipeline> pipeline) = 0;

        //Draw
		virtual void Draw(size_t count, Ref<GPUBuffer> indexBuffer = nullptr) = 0;
		virtual void DrawInstanced(size_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer = nullptr) = 0;
        virtual void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset = 0, Ref<GPUBuffer> indexBuffer = nullptr) = 0;
		virtual void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset = 0, uint32_t maxDraws = 0, Ref<GPUBuffer> indexBuffer = nullptr) = 0;
        //Compute
		virtual void DispatchCompute(GroupSize GlobalGroupSize) = 0;
		virtual void TraceRays(const TraceRaysInfo& traceRaysInfo) = 0;

        //Barriers
		//virtual void InsertMemoryBarrier(const MemoryBarrierInfo& info) = 0;
        
		//Resource transitions
		//virtual void TransitionImageLayout(const ImageTransitionInfo& info) = 0;
		//virtual void GenerateMipmaps(const GenMipmapsInfo& info) = 0;
		//virtual void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) = 0;

		//Statistics
		
		

		virtual uint32_t GetFrameIndex() = 0;
		virtual bool IsRecording() = 0;

    public:
		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", DeviceQueue queue = DeviceQueue::GraphicsBit, uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}

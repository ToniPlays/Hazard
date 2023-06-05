#pragma once

#include "Core.h"
#include "RenderContextCreateInfo.h"

#include "RenderPass.h"
#include "Pipeline.h"
#include "Texture.h"
#include "RTCore/ShaderBindingTable.h"

namespace HazardRenderer 
{
	using LocalGroupSize = glm::vec3;
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

	struct DispatchComputeInfo
	{
		LocalGroupSize GroupSize;
		bool WaitForCompletion = false;
	};

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

	struct TraceRaysInfo
	{
		Extent Extent;
		Ref<ShaderBindingTable> pBindingTable;
	};
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
		virtual void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding = 0) = 0;
		virtual void SetUniformBuffers(const Ref<UniformBuffer>* uniformBuffer, uint32_t count) = 0;
		virtual void SetPipeline(Ref<Pipeline> pipeline) = 0;

        //Draw
		virtual void Draw(size_t count, Ref<IndexBuffer> indexBuffer = nullptr) = 0;
		virtual void DrawInstanced(size_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer = nullptr) = 0;
        virtual void DrawIndirect(uint32_t drawCount, uint32_t offset = 0) = 0;
        
        //Compute
		virtual void DispatchCompute(const DispatchComputeInfo& computeInfo) = 0;
        
		virtual void TraceRays(const TraceRaysInfo& traceRaysInfo) = 0;

        //Barriers
		virtual void InsertMemoryBarrier(const MemoryBarrierInfo& info) = 0;
        //Resource transitions
		virtual void TransitionImageLayout(const ImageTransitionInfo& info) = 0;
		virtual void GenerateMipmaps(const GenMipmapsInfo& info) = 0;
		virtual void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) = 0;

		virtual uint32_t GetFrameIndex() = 0;
		virtual bool IsRecording() = 0;

    public:
		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", DeviceQueue queue = DeviceQueue::GraphicsBit, uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}

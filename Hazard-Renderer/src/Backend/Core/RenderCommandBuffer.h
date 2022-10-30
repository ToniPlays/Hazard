#pragma once

#include "Core.h"
#include "RenderContextCreateInfo.h"

#include "Pipeline/RenderPass.h"
#include "Pipeline/Pipeline.h"

namespace HazardRenderer 
{
	using LocalGroupSize = glm::vec3;
	enum class State { Waiting, Record, Finished, Submit };

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
		Ref<Pipeline> Pipeline;
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

		virtual void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding = 0) = 0;
		virtual void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) = 0;
		virtual void BindPipeline(Ref<Pipeline> pipeline) = 0;

		virtual void Draw(uint32_t count, Ref<IndexBuffer> indexBuffer = nullptr) = 0;
		virtual void DrawInstanced(uint32_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer = nullptr) = 0;
		virtual void DispatchCompute(const DispatchComputeInfo& computeInfo) = 0;

		virtual void InsertMemoryBarrier(const MemoryBarrierInfo& info) = 0;
		virtual void TransitionImageLayout(const ImageTransitionInfo& info) = 0;

		virtual void SetViewport(float x, float y, float width, float height) = 0;
		virtual void SetLineSize(float size) = 0;

		virtual uint32_t GetFrameIndex() = 0;
		virtual bool IsRecording() = 0;

		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}
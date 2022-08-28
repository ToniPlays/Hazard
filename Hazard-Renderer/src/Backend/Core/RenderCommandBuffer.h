#pragma once

#include "Core.h"
#include "RenderContextCreateInfo.h"

#include "Pipeline/RenderPass.h"
#include "Pipeline/Pipeline.h"

namespace HazardRenderer 
{
	enum class State { Waiting, Record, Finished, Submit };

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

		virtual void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void BindIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;
		virtual void BindPipeline(Ref<Pipeline> pipeline) = 0;

		virtual void SetViewport(float x, float y, float width, float height) = 0;
		virtual void SetLineSize(float size) = 0;

		virtual uint32_t GetFrameIndex() = 0;
		virtual bool IsRecording() = 0;

		static Ref<RenderCommandBuffer> Create(const std::string& debugName = "", uint32_t count = 0);
		static Ref<RenderCommandBuffer> CreateFromSwapchain(const std::string& debugName = "");
	};
}
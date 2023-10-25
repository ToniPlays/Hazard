#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Core/RenderCommandBuffer.h"
#include "Core/Swapchain.h"
#include "OpenGL/OpenGLPipeline.h"

namespace HazardRenderer::OpenGL 
{
	class OpenGLRenderCommandBuffer : public RenderCommandBuffer 
	{
	public:
		OpenGLRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t size);
		OpenGLRenderCommandBuffer(const std::string& name, bool swapchain);
		~OpenGLRenderCommandBuffer() = default;

		virtual uint32_t GetFrameIndex() { return m_FrameIndex; };
		virtual bool IsRecording() { return true;  };

		void Begin();
		void End() {};
		void Submit() {};

		void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false) override;
		void BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear = false);
		void EndRenderPass() override;

		void SetVertexBuffer(Ref<GPUBuffer> GPUBuffer, uint32_t binding = 0) override;
		//void SetUniformBuffers(const Ref<GPUBuffer>* uniformBuffer, uint32_t count) override;
		void SetPipeline(Ref<Pipeline> pipeline) override;

		void Draw(uint64_t count, Ref<GPUBuffer> indexBuffer = nullptr) override;
		void DrawInstanced(uint64_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer = nullptr) override;
		void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset = 0, Ref<GPUBuffer> indexBuffer = nullptr) override {
			HZR_ASSERT(false, "Not implemented");
		};

		void DispatchCompute(GroupSize globalGroupSize) override;
		void TraceRays(const TraceRaysInfo& traceRaysInfo) override {};
		void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) {};

		//void InsertMemoryBarrier(const MemoryBarrierInfo& info) override;
		//void TransitionImageLayout(const ImageTransitionInfo& info) override {};
		//void GenerateMipmaps(const GenMipmapsInfo& info) override {};

	private:
		uint32_t m_FrameIndex = 0;
		Ref<Swapchain> m_Swapchain;
		Ref<OpenGLPipeline> m_CurrentPipeline;

		inline static BufferLayout s_CurrentLayout;
		inline static uint32_t s_BoundVAO;
	};
}
#endif

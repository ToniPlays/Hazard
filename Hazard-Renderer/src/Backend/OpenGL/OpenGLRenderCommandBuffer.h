#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/RenderCommandBuffer.h"
#include "Backend/Core/Swapchain.h"
#include "Backend/OpenGL/Pipeline/OpenGLPipeline.h"


namespace HazardRenderer::OpenGL 
{
	class OpenGLRenderCommandBuffer : public RenderCommandBuffer {
	public:
		OpenGLRenderCommandBuffer(uint32_t size, const std::string& name, bool compute = false);
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

		void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding = 0) override;
		void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) override;
		void BindPipeline(Ref<Pipeline> pipeline) override;

		void Draw(uint32_t count, Ref<IndexBuffer> indexBuffer = nullptr) override;
		void DrawInstanced(uint32_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer = nullptr) override;
		void DispatchCompute(const DispatchComputeInfo& computeInfo) override;
		void TraceRays(const TraceRaysInfo& traceRaysInfo) override {};
		void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) {};

		void InsertMemoryBarrier(const MemoryBarrierInfo& info) override;
		void TransitionImageLayout(const ImageTransitionInfo& info) override {};
		void GenerateMipmaps(const GenMipmapsInfo& info) override {};

		void SetViewport(float x, float y, float width, float height) override {};
		void SetLineSize(float size) override {};

	private:
		uint32_t m_FrameIndex = 0;
		Ref<Swapchain> m_Swapchain;
		Ref<OpenGLPipeline> m_CurrentPipeline;

		inline static uint32_t s_BoundVAO = 0;
		inline static BufferLayout s_CurrentLayout;
	};
}
#endif

#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/RenderCommandBuffer.h"
#include "Pipeline/MetalPipeline.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalRenderCommandBuffer : public RenderCommandBuffer
    {
    public:
        MetalRenderCommandBuffer(uint32_t count = 0, const std::string& name = "", bool compute = false);
        MetalRenderCommandBuffer(const std::string& name, bool swapchain);
        ~MetalRenderCommandBuffer();

        uint32_t GetFrameIndex() override { return m_FrameIndex; };
        
        bool IsRecording() override { return m_State == State::Record; };
        void Begin() override;
        void End() override;
        void Submit() override;

        void BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear = false) override;
        void BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear = false);
        void EndRenderPass() override;

        void BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding) override;
        void BindUniformBuffer(Ref<UniformBuffer> uniformBuffer) override;
        void BindPipeline(Ref<Pipeline> pipeline) override;

        void Draw(size_t count, Ref<IndexBuffer> indexBuffer = nullptr) override;
        void DrawInstanced(size_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer = nullptr) override;
        void DispatchCompute(const DispatchComputeInfo& computeInfo) override;
        void TraceRays(const TraceRaysInfo& traceRaysInfo) override
        {
            HZR_ASSERT(false, "Not implemented")
        };
        void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) override {
            HZR_ASSERT(false, "Not implemented")
        };

        void InsertMemoryBarrier(const MemoryBarrierInfo& info) override {
            HZR_ASSERT(false, "Not implemented")
        };
        void TransitionImageLayout(const ImageTransitionInfo& info) override {
            HZR_ASSERT(false, "Not implemented")
        };
        void GenerateMipmaps(const GenMipmapsInfo& info) override;

        void SetViewport(float x, float y, float width, float height) override {
            HZR_ASSERT(false, "Not implemented")
        };
        void SetLineSize(float size) override;
        
        //Metal specific
        MTL::CommandBuffer* GetMetalCommandBuffer() const { return m_CommandBuffer; }
        MTL::RenderCommandEncoder* GetRenderEncoder() const { return m_RenderEncoder; }
        MTL::ComputeCommandEncoder* GetComputeEncoder() const { return m_ComputeEncoder; }
        
    private:
        uint32_t m_FrameIndex = 0;
        std::string m_DebugName;
        bool m_OwnedBySwapchain = false;
        bool m_IsCompute = false;
        bool m_WaitOnSubmit = false;
        Ref<MetalPipeline> m_CurrentPipeline = nullptr;
        State m_State = State::Waiting;
        
        MTL::CommandBuffer* m_CommandBuffer;
        MTL::RenderCommandEncoder* m_RenderEncoder;
        MTL::ComputeCommandEncoder* m_ComputeEncoder;
    };
}
#endif

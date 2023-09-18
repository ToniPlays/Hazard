#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/RenderCommandBuffer.h"
#include "MetalPipeline.h"
#include "MetalSwapchain.h"
#include "MetalTopLevelAS.h"
#include "MetalBottomLevelAS.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalRenderCommandBuffer : public RenderCommandBuffer
    {
    public:
        MetalRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t count);
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
        
        void SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding) override;
        void SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set = 0) override {}
        void PushConstants(Buffer buffer, uint32_t offset, uint32_t flags) override {}
        
        void SetPipeline(Ref<Pipeline> pipeline) override;
        
        void Draw(size_t count, Ref<GPUBuffer> indexBuffer = nullptr) override;
        void DrawInstanced(size_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer = nullptr) override;
        void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset = 0, Ref<GPUBuffer> indexBuffer = nullptr) override;
        void DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset = 0, uint32_t maxDraws = 0, Ref<GPUBuffer> indexBuffer = nullptr) override;
        
        
        void DispatchCompute(GroupSize GlobalGroupSize) override;
        void TraceRays(const TraceRaysInfo& traceRaysInfo) override;
          
    public:
        //Metal specific
        MTL::CommandBuffer* GetMetalCommandBuffer() const { return m_CommandBuffer; }
        MTL::RenderCommandEncoder* GetRenderEncoder() const { return m_RenderEncoder; }
        MTL::ComputeCommandEncoder* GetComputeEncoder() const { return m_ComputeEncoder; }
            
    private:
        std::string m_DebugName;
        uint32_t m_FrameIndex = 0;
        DeviceQueue m_Queue;
        bool m_OwnedBySwapchain = false;
        bool m_WaitOnSubmit = false;
        Ref<MetalPipeline> m_CurrentPipeline = nullptr;
        State m_State = State::Waiting;
            
        MTL::CommandBuffer* m_CommandBuffer;
        MTL::IndirectCommandBuffer* m_IndirectCommandBuffer;
        MTL::RenderCommandEncoder* m_RenderEncoder = nullptr;
        MTL::ComputeCommandEncoder* m_ComputeEncoder = nullptr;
        MTL::AccelerationStructureCommandEncoder* m_AccelerationEcoder;
    };
}
#endif

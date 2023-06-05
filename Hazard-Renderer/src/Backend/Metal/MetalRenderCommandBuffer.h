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
        
        void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding) override;
        void SetUniformBuffers(const Ref<UniformBuffer>* uniformBuffer, uint32_t count) override;
        void SetPipeline(Ref<Pipeline> pipeline) override;
        
        void Draw(size_t count, Ref<IndexBuffer> indexBuffer = nullptr) override;
        void DrawInstanced(size_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer = nullptr) override;
        void DrawIndirect(uint32_t drawCount, uint32_t offset = 0) override;
        
        
        void DispatchCompute(const DispatchComputeInfo& computeInfo) override;
        void TraceRays(const TraceRaysInfo& traceRaysInfo) override;
        void BuildAccelerationStructure(const AccelerationStructureBuildInfo& info) override;
        
        void InsertMemoryBarrier(const MemoryBarrierInfo& info) override {};
        void TransitionImageLayout(const ImageTransitionInfo& info) override {};
        void GenerateMipmaps(const GenMipmapsInfo& info) override;
          
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

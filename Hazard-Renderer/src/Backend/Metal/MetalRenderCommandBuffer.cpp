
#include "MetalRenderCommandBuffer.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Window.h"
#include "Backend/Core/Renderer.h"

#include "MetalCore.h"

namespace HazardRenderer::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t count)
    {
        m_DebugName = name;
        m_Queue = queue;
        m_OwnedBySwapchain = false;
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain)
    {
        m_DebugName = name;
        m_OwnedBySwapchain = swapchain;
        m_Queue = DeviceQueue::GraphicsBit;
    }
    MetalRenderCommandBuffer::~MetalRenderCommandBuffer()
    {
        if(m_CommandBuffer)
            m_CommandBuffer->release();
        if(m_IndirectCommandBuffer)
            m_IndirectCommandBuffer->release();
        
        if(m_ComputeEncoder)
            m_ComputeEncoder->release();
    }
    void MetalRenderCommandBuffer::Begin()
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            auto device = MetalContext::GetMetalDevice();
            instance->m_CommandBuffer = device->GetGraphicsQueue()->commandBuffer();
            
            /*MTL::IndirectCommandBufferDescriptor* descriptor = MTL::IndirectCommandBufferDescriptor::alloc()->init();
            
            instance->m_IndirectCommandBuffer = device->GetMetalDevice()->newIndirectCommandBuffer(descriptor, 512, MTL::ResourceStorageModeManaged);
                */
            //descriptor->release();
            if(instance->m_Queue == DeviceQueue::ComputeBit)
                instance->m_ComputeEncoder = instance->m_CommandBuffer->computeCommandEncoder();
            
            
        });
    }
    void MetalRenderCommandBuffer::End()
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            if(instance->m_ComputeEncoder)
                instance->m_ComputeEncoder->endEncoding();
        });
    }
    void MetalRenderCommandBuffer::Submit()
    {
        if(m_OwnedBySwapchain) return;
        
        Ref<MetalRenderCommandBuffer> instance = this;
        
        Renderer::Submit([instance, wait = m_WaitOnSubmit]() mutable {
            instance->m_CommandBuffer->commit();
            instance->m_CommandBuffer->waitUntilCompleted();
        });
    }

    void MetalRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance, renderPass]() mutable {
            instance->BeginRenderPass_RT(renderPass);
        });
    }
    void MetalRenderCommandBuffer::BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear)
    {
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        auto& specs = fb->GetSpecification();
        MTL::RenderPassDescriptor* descriptor = fb->GetMetalRenderPassDescriptor();
        
        float width = fb->GetWidth();
        float height = fb->GetHeight();
        
        MTL::Viewport viewport;
        viewport.originX = 0.0;
        viewport.originY = 0.0;
        viewport.znear = 0.0f;
        viewport.zfar = 1.0f;
        
        if(specs.SwapChainTarget)
        {
            auto swapchain = MetalContext::GetInstance()->GetSwapchain().As<MetalSwapchain>();
            descriptor->setRenderTargetWidth(swapchain->GetWidth());
            descriptor->setRenderTargetHeight(swapchain->GetHeight());
            descriptor->setDefaultRasterSampleCount(1);
            
            auto clearColor = MetalContext::GetInstance()->GetClearColorValue();
            
            auto c1 = descriptor->colorAttachments()->object(0);
            c1->init();
            c1->setClearColor(clearColor);
            c1->setTexture(swapchain->GetDrawable()->texture());
            c1->setLoadAction(MTL::LoadActionClear);
            c1->setStoreAction(MTL::StoreActionStore);
            
            width = swapchain->GetWidth();
            height = swapchain->GetHeight();
            
            viewport.width = width;
            viewport.height = height;
        }
        else
        {
            descriptor->setRenderTargetWidth(width);
            descriptor->setRenderTargetHeight(height);
            descriptor->setDefaultRasterSampleCount(specs.Samples);
            
            for(uint32_t i = 0; i < fb->GetColorAttachmentCount(); i++)
            {
                Ref<MetalImage2D> image = fb->GetImage(i).As<MetalImage2D>();
                
                auto color = MTL::ClearColor(specs.ClearColor.r, specs.ClearColor.g, specs.ClearColor.b, specs.ClearColor.a);
                
                auto colorAttachment = descriptor->colorAttachments()->object(i);
                colorAttachment->init();
                colorAttachment->setClearColor(color);
                colorAttachment->setTexture(image->GetMetalTexture());
                colorAttachment->setLoadAction(MTL::LoadActionClear);
                colorAttachment->setStoreAction(MTL::StoreActionStore);
            }
            
            if(fb->GetDepthImage())
            {
                MTL::RenderPassDepthAttachmentDescriptor* depthDescriptor = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();
                
                depthDescriptor->setClearDepth(1.0);
                depthDescriptor->setLoadAction(specs.ClearOnLoad ? MTL::LoadActionClear : MTL::LoadActionDontCare);
                depthDescriptor->setStoreAction(MTL::StoreActionStore);
                depthDescriptor->setTexture(fb->GetDepthImage().As<MetalImage2D>()->GetMetalTexture());
                descriptor->setDepthAttachment(depthDescriptor);
            }
            viewport.originY = height;
            viewport.width = width;
            viewport.height = -height;
        }
        
        m_RenderEncoder = m_CommandBuffer->renderCommandEncoder(descriptor);
        
        MTL::ScissorRect scissors;
        scissors.width = width;
        scissors.height = height;
        scissors.x = 0.0;
        scissors.y = 0.0;
        
        m_RenderEncoder->setViewport(viewport);
        m_RenderEncoder->setScissorRect(scissors);
    }
    void MetalRenderCommandBuffer::EndRenderPass()
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            instance->m_RenderEncoder->endEncoding();
        });
    }
    void MetalRenderCommandBuffer::SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalGPUBuffer> buffer = vertexBuffer.As<MetalGPUBuffer>();
        Renderer::Submit([instance, buffer, binding]() mutable {
            instance->m_RenderEncoder->setVertexBuffer(buffer->GetMetalBuffer(), 0, 28 + binding);
        });
    }
    void MetalRenderCommandBuffer::SetPipeline(Ref<Pipeline> pipeline)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalPipeline> metalPipeline = pipeline.As<MetalPipeline>();
        
        Renderer::Submit([instance, metalPipeline]() mutable {
            instance->m_CurrentPipeline = metalPipeline;
            
            PipelineUsage usage = metalPipeline->GetSpecifications().Usage;
            if(usage == PipelineUsage::GraphicsBit)
                metalPipeline->BindGraphics(instance->m_RenderEncoder);
            else
                metalPipeline->BindCompute(instance->m_ComputeEncoder);
        });
    }
    void MetalRenderCommandBuffer::Draw(size_t count, Ref<GPUBuffer> indexBuffer)
    {
        DrawInstanced(count, 1, indexBuffer);
    }
    void MetalRenderCommandBuffer::DrawInstanced(size_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalGPUBuffer> mtlIndexBuffer = indexBuffer.As<MetalGPUBuffer>();
        Renderer::Submit([instance, mtlIndexBuffer, count, instanceCount]() mutable {
            
            auto primitiveType = instance->m_CurrentPipeline->GetMetalPrimitiveType();
            if(mtlIndexBuffer)
            {
                MTL::Buffer* buffer = mtlIndexBuffer->GetMetalBuffer();
                instance->m_RenderEncoder->drawIndexedPrimitives(primitiveType, count, MTL::IndexTypeUInt32, buffer, 0, instanceCount, 0, 0);
            }
            else
            {
                instance->m_RenderEncoder->drawPrimitives(primitiveType, 0, count, instanceCount);
            }
        });
    }
    void MetalRenderCommandBuffer::DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t drawCount, uint32_t stride, uint32_t offset, Ref<GPUBuffer> indexBuffer) {
        
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalGPUBuffer> argBuffer = argumentBuffer.As<MetalGPUBuffer>();
        Ref<MetalGPUBuffer> mtlIndexBuffer = indexBuffer.As<MetalGPUBuffer>();
        
        Renderer::Submit([instance, argBuffer, mtlIndexBuffer, drawCount, stride, offset]() mutable {
            
            auto argBuf = argBuffer->GetMetalBuffer();
            
            if(mtlIndexBuffer)
            {
                auto buffer = mtlIndexBuffer->GetMetalBuffer();
                for(uint32_t i = 0; i < drawCount; i++)
                {
                    instance->m_RenderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, MTL::IndexTypeUInt32, buffer, 0, argBuf, stride * i + offset);
                    instance->m_RenderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, MTL::IndexTypeUInt32, buffer, 0, argBuf, stride * i + offset);
                }
            }
            else
            {
                
            }
        });
    }
    void MetalRenderCommandBuffer::DispatchCompute(GroupSize GlobalGroupSize)
    {
        m_WaitOnSubmit = true;
        Ref<MetalRenderCommandBuffer> instance = this;
        
        if(!m_ComputeEncoder)
            m_ComputeEncoder = m_CommandBuffer->computeCommandEncoder();
        
        Renderer::Submit([instance, size = GlobalGroupSize]() mutable {
        
            MTL::Size localGroup = { 32, 32, 1 };
            MTL::Size groupSize = {
                static_cast<NS::UInteger>(size.x),
                static_cast<NS::UInteger>(size.y),
                static_cast<NS::UInteger>(size.z)
            };
            auto encoder = instance->m_ComputeEncoder;
            encoder->dispatchThreadgroups(groupSize, localGroup);
        });
    }
    void MetalRenderCommandBuffer::TraceRays(const TraceRaysInfo& traceRaysInfo)
    {
        GroupSize groupSize = {};
        groupSize.x = traceRaysInfo.Extent.Width;
        groupSize.y = traceRaysInfo.Extent.Height;
        groupSize.z = traceRaysInfo.Extent.Depth;
        
        DispatchCompute(groupSize);
    }
/*
    void MetalRenderCommandBuffer::BuildAccelerationStructure(const AccelerationStructureBuildInfo &info)
    {
        Ref<AccelerationStructure> structure = info.AccelerationStructure;
        structure->Invalidate();

        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance, buildInfo = info]() mutable {

            auto level = buildInfo.AccelerationStructure->GetLevel();

            if (level == AccelerationStructureLevel::Top)
                buildInfo.AccelerationStructure.As<MetalTopLevelAS>()->Build(instance->m_AccelerationEcoder, buildInfo.Type);
            else if (level == AccelerationStructureLevel::Bottom)
                buildInfo.AccelerationStructure.As<MetalBottomLevelAS>()->Build(instance->m_AccelerationEcoder, buildInfo.Type);
            });
    }
    void MetalRenderCommandBuffer::GenerateMipmaps(const GenMipmapsInfo &info)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance, mips = info]() mutable {

            if (mips.Cubemap)
                mips.Cubemap.As<MetalCubemapTexture>()->GenerateMipmaps_RT(instance->m_CommandBuffer);
            });
    }
 */
}
#endif

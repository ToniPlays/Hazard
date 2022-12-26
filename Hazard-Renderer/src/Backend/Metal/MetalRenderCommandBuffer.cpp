
#include "MetalRenderCommandBuffer.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Window.h"
#include "Backend/Core/Renderer.h"

#include "MetalContext.h"
#include "MetalSwapchain.h"
#include "MetalRenderPass.h"
#include "MetalFrameBuffer.h"
#include "MetalPipeline.h"
#include "MetalBuffers.h"

namespace HazardRenderer::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(uint32_t count, const std::string& name, bool compute)
    {
        
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain)
    {
        m_DebugName = name;
        m_OwnedBySwapchain = swapchain;
    }
    MetalRenderCommandBuffer::~MetalRenderCommandBuffer()
    {
        m_CommandBuffer->release();
    }
    void MetalRenderCommandBuffer::Begin()
    {
        auto device = MetalContext::GetMetalDevice();
        m_CommandBuffer = device->GetGraphicsQueue()->commandBuffer();
    }
    void MetalRenderCommandBuffer::End()
    {
        
    }
    void MetalRenderCommandBuffer::Submit()
    {
        if(m_OwnedBySwapchain) return;
        m_CommandBuffer->commit();
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
        
        if(specs.SwapChainTarget)
        {
            auto swapchain = MetalContext::GetInstance()->GetSwapchain().As<MetalSwapchain>();
            descriptor->setRenderTargetWidth(swapchain->GetWidth());
            descriptor->setRenderTargetHeight(swapchain->GetHeight());
            
            auto clearColor = MetalContext::GetInstance()->GetClearColorValue();
            
            auto c1 = descriptor->colorAttachments()->object(0);
            c1->init();
            c1->setClearColor(clearColor);
            c1->setTexture(swapchain->GetDrawable()->texture());
            c1->setLoadAction(MTL::LoadActionClear);
            c1->setStoreAction(MTL::StoreActionStore);
        }
        else
        {
            descriptor->setRenderTargetWidth(fb->GetWidth());
            descriptor->setRenderTargetHeight(fb->GetHeight());
        }
        
        m_RenderEncoder = m_CommandBuffer->renderCommandEncoder(descriptor);
    
        MTL::Viewport viewport;
        viewport.width = descriptor->renderTargetWidth();
        viewport.height = descriptor->renderTargetHeight();
        viewport.originX = 0.0;
        viewport.originY = 0.0;
        
        MTL::ScissorRect scissors;
        scissors.width = viewport.width;
        scissors.height = viewport.height;
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
    void MetalRenderCommandBuffer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalVertexBuffer> buffer = vertexBuffer.As<MetalVertexBuffer>();
        Renderer::Submit([instance, buffer, binding]() mutable {
            instance->m_RenderEncoder->setVertexBuffer(buffer->GetMetalBuffer(), 0, 28 + binding);
        });
    }
    void MetalRenderCommandBuffer::BindUniformBuffer(Ref<UniformBuffer> uniformBuffer)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalUniformBuffer> metalUniformBuffer = uniformBuffer.As<MetalUniformBuffer>();
        
        Renderer::Submit([instance, metalUniformBuffer]() mutable {
            
        });
    }
    void MetalRenderCommandBuffer::BindPipeline(Ref<Pipeline> pipeline)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalPipeline> metalPipeline = pipeline.As<MetalPipeline>();
        
        Renderer::Submit([instance, metalPipeline]() mutable {
            instance->m_CurrentPipeline = metalPipeline;
            metalPipeline->Bind(instance->m_RenderEncoder);
        });
    }
    void MetalRenderCommandBuffer::Draw(size_t count, Ref<IndexBuffer> indexBuffer)
    {
        DrawInstanced(count, 1, indexBuffer);
    }
    void MetalRenderCommandBuffer::DrawInstanced(size_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalIndexBuffer> mtlIndexBuffer = indexBuffer.As<MetalIndexBuffer>();
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
}
#endif

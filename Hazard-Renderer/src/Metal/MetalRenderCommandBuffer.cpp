
#include "MetalRenderCommandBuffer.h"

#ifdef HZR_INCLUDE_METAL

#include "Core/Window.h"
#include "Core/Renderer.h"

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
        Renderer::SubmitResourceFree([commandBuffer = m_CommandBuffer, indirectCommandBuffer = m_IndirectCommandBuffer, computeEncoder = m_ComputeEncoder]() mutable {
            
             if(commandBuffer)
             commandBuffer->release();
             if(indirectCommandBuffer)
             indirectCommandBuffer->release();
             if(computeEncoder)
             computeEncoder->release();
         });
    }
    void MetalRenderCommandBuffer::Begin()
    {
        if(m_OwnedBySwapchain)
        {
            auto device = MetalContext::GetMetalDevice();
            m_CommandBuffer = device->GetGraphicsQueue()->commandBuffer();
            return;
        }
        
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            auto device = MetalContext::GetMetalDevice();
            instance->m_CommandBuffer = device->GetGraphicsQueue()->commandBuffer();
            
            if(instance->m_Queue == DeviceQueue::ComputeBit)
            {
                instance->m_ComputeEncoder = instance->m_CommandBuffer->computeCommandEncoder();
            }
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
            
            auto color = MTL::ClearColor(specs.ClearColor.r, specs.ClearColor.g, specs.ClearColor.b, specs.ClearColor.a);
            
            auto c1 = descriptor->colorAttachments()->object(0);
            c1->init();
            c1->setClearColor(color);
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
                
                depthDescriptor->release();
            }
            viewport.originY = height;
            viewport.width = width;
            viewport.height = -height;
        }
        
        MTL::ScissorRect scissors;
        scissors.width = width;
        scissors.height = height;
        scissors.x = 0.0;
        scissors.y = 0.0;
        
        
        m_RenderEncoder = m_CommandBuffer->renderCommandEncoder(descriptor);
        
        m_RenderEncoder->setViewport(viewport);
        m_RenderEncoder->setScissorRect(scissors);
    }
    void MetalRenderCommandBuffer::EndRenderPass()
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            instance->m_RenderEncoder->endEncoding();
        });
        
        Renderer::SubmitResourceFree([encoder = m_RenderEncoder]() mutable {
            
            encoder->release();
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
    void MetalRenderCommandBuffer::SetDescriptorSet(Ref<DescriptorSet> descriptorSet, uint32_t set)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalDescriptorSet> mtlSet = descriptorSet.As<MetalDescriptorSet>();
        
        Renderer::Submit([instance, mtlSet, set]() mutable {
            if(instance->m_Queue == DeviceQueue::GraphicsBit)
                mtlSet->BindGraphicsResources(instance->m_RenderEncoder);
            if(instance->m_Queue == DeviceQueue::ComputeBit)
                mtlSet->BindComputeResources(instance->m_ComputeEncoder);
        });
    }
    void MetalRenderCommandBuffer::PushConstants(Buffer buffer, uint32_t offset, uint32_t flags)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Buffer dataBuffer = Buffer::Copy(buffer);
        
        Renderer::Submit([instance, buffer = dataBuffer, offset, flags]() mutable {
            if(instance->m_Queue == DeviceQueue::GraphicsBit)
            {
                HZR_ASSERT(!(flags & SHADER_STAGE_COMPUTE_BIT), "Invalid flag combination")
                if(flags & SHADER_STAGE_VERTEX_BIT)
                    instance->m_RenderEncoder->setVertexBytes(buffer.Data, buffer.Size, 0);
                if(flags & SHADER_STAGE_FRAGMENT_BIT)
                    instance->m_RenderEncoder->setFragmentBytes(buffer.Data, buffer.Size, 0);
            }
            if(instance->m_Queue == DeviceQueue::ComputeBit) {
                if(flags & SHADER_STAGE_COMPUTE_BIT)
                    instance->m_ComputeEncoder->setBytes(buffer.Data, buffer.Size, 0);
            }
            buffer.Release();
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
            else if(usage == PipelineUsage::ComputeBit)
                metalPipeline->BindCompute(instance->m_ComputeEncoder);
        });
    }
    void MetalRenderCommandBuffer::SetLineWidth(float width)
    {
        
    }

    void MetalRenderCommandBuffer::Draw(uint64_t count, Ref<GPUBuffer> indexBuffer)
    {
        DrawInstanced(count, 1, indexBuffer);
    }
    void MetalRenderCommandBuffer::DrawInstanced(uint64_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer)
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

    void MetalRenderCommandBuffer::DrawIndirect(Ref<GPUBuffer> argumentBuffer, uint32_t stride, uint32_t offset, Ref<GPUBuffer> drawCountBuffer, uint32_t drawCountOffset, uint32_t maxDraws, Ref<GPUBuffer> indexBuffer)
    {
        
    };


    void MetalRenderCommandBuffer::CopyToBuffer(Ref<GPUBuffer> targetBuffer, const BufferCopyRegion& region)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Ref<MetalGPUBuffer> buffer = targetBuffer.As<MetalGPUBuffer>();
        Buffer data = Buffer::Copy(region.Data, region.Size);
        
        Renderer::Submit([instance, buffer, data, offset = region.Offset]() mutable {
            void* dst = buffer->GetMetalBuffer()->contents();
            memcpy((uint8_t*)dst, data.Data, data.Size);
            data.Release();
        });
    }
    void MetalRenderCommandBuffer::CopyToImage(Ref<Image> targetImage, const ImageCopyRegion& region)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Buffer buffer = Buffer::Copy(region.Data, region.DataSize);
        
        Renderer::Submit([instance, targetImage, region, buffer]() mutable {
            HZR_PROFILE_FUNCTION();
            
            MTL::Texture* texture = targetImage->GetType() == TextureType::Image2D ? targetImage.As<MetalImage2D>()->GetMetalTexture() : targetImage.As<MetalCubemapTexture>()->GetMetalTexture();
            
            MTL::Region mtlRegion;
            mtlRegion.size.width = region.Extent.Width;
            mtlRegion.size.height = region.Extent.Height;
            mtlRegion.size.depth = region.Extent.Depth;
            mtlRegion.origin.x = region.X;
            mtlRegion.origin.y = region.Y;
            mtlRegion.origin.z = region.Z;
            
            texture->replaceRegion(mtlRegion, 0, buffer.Data, 4 * region.Extent.Width);
            buffer.Release();
        });
    }
    void MetalRenderCommandBuffer::BlitImage(const BlitImageInfo& blitInfo)
    {
        if(!m_BlitComputePipeline)
        {
            std::string shaderSource = blitInfo.Image->GetType() == TextureType::Image2D ? R"(
                        kernel void main0(constant BlitInfo& info [[buffer(0)]], texture2d<float> src [[texture(0)]], texture2d<float, access::write> dst [[texture(1)]], uint3 gl_GlobalInvocationID [[thread_position_in_grid]], uint3 gl_NumWorkGroups [[threadgroups_per_grid]])
                        {
                            
                            int2 texelCoord = int2(gl_GlobalInvocationID.xy);
                            float2 sampleCoord = float2(float(texelCoord.x) / float(info.DstExtent.x),
                                                        float(texelCoord.y) / float(info.DstExtent.y));
                    
                            float4 value = src.sample(smplr, sampleCoord, level(info.SrcMip));
                            dst.write(value, uint2(texelCoord), info.DstMip);
                        }
                    )" : R"(
                        kernel void main0(constant BlitInfo& info [[buffer(0)]], texturecube<float> src [[texture(0)]], texturecube<float, access::write> dst [[texture(1)]], uint3 gl_GlobalInvocationID [[thread_position_in_grid]], uint3 gl_NumWorkGroups [[threadgroups_per_grid]])
                        {
                            
                            int2 texelCoord = int2(gl_GlobalInvocationID.xy);
                            float2 coord = float2(float(texelCoord.x) / float(info.DstExtent.x),
                                                1.0 - float(texelCoord.y) / float(info.DstExtent.y));
                            
                            float3 samplerCoord = float3(0.0);
                            
                            switch(info.SrcLayer)
                            {
                                case 0: 
                                    samplerCoord = float3(1.0, coord.y, 1.0 - coord.x);
                                break;
                                case 1: 
                                    samplerCoord = float3(-1.0, coord.y, coord.x);
                                break;
                                case 2:
                                    samplerCoord = float3(coord.x, 1.0, 1.0 - coord.y);
                                break;
                                case 3: 
                                    samplerCoord = float3(coord.x, -1.0, coord.y);
                                break;
                                case 4:
                                    samplerCoord = float3(coord.x, coord.y, 1.0);
                                break;
                                case 5: 
                                    samplerCoord = float3(1.0 - coord.x, coord.y, -1.0);
                                break;
                            }
                            
                            float4 value = src.sample(smplr, samplerCoord * 2.0 - 1.0, level(info.SrcMip));
                            dst.write(value, uint2(texelCoord.xy), info.DstLayer, info.DstMip);
                        })";
            
            
            PipelineSpecification spec = {};
            spec.DebugName = "BlitCompute";
            spec.Usage = PipelineUsage::ComputeBit;
            spec.PushConstants = {};
            spec.Shaders = {
                { SHADER_STAGE_COMPUTE_BIT, R"(
                            #include <metal_stdlib>
                            #include <simd/simd.h>
                                                                                    
                            using namespace metal;
                                                                                
                            struct BlitInfo {
                            float4 SrcExtent;
                            float4 DstExtent;
                            uint SrcLayer;
                            uint DstLayer;
                            uint SrcMip;
                            uint DstMip;
                            };
                                                                                    
                            constant uint3 gl_WorkGroupSize [[maybe_unused]] = uint3(1u);
                            constexpr sampler smplr (mag_filter::linear,
                            min_filter::linear);)" + shaderSource }
            };
            m_BlitComputePipeline = Pipeline::Create(&spec).As<MetalPipeline>();
        }
        
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance, blitInfo]() mutable {
            if(!instance->m_ComputeEncoder)
                instance->m_ComputeEncoder = instance->m_CommandBuffer->computeCommandEncoder();
        });
        
        SetPipeline(m_BlitComputePipeline);
        
        Renderer::Submit([instance, blitInfo]() mutable {
            struct BlitInfo {
                glm::vec4 SrcExtent;
                glm::vec4 DstExtent;
                uint32_t SrcLayer;
                uint32_t DstLayer;
                uint32_t SrcMip;
                uint32_t DstMip;
                
            } info;
            
            info.SrcExtent = { blitInfo.SrcExtent.Width, blitInfo.SrcExtent.Height, blitInfo.SrcExtent.Depth, 0.0f };
            info.DstExtent = { blitInfo.DstExtent.Width, blitInfo.DstExtent.Height, blitInfo.DstExtent.Depth, 0.0f };
            info.SrcLayer = blitInfo.SrcLayer;
            info.DstLayer = blitInfo.DstLayer;
            info.SrcMip = blitInfo.SrcMip;
            info.DstMip = blitInfo.DstMip;
            
            MTL::Texture* texture = blitInfo.Image->GetType() == TextureType::Image2D ? blitInfo.Image.As<MetalImage2D>()->GetMetalTexture() : blitInfo.Image.As<MetalCubemapTexture>()->GetMetalTexture();
            
            instance->m_ComputeEncoder->setBytes(&info, sizeof(blitInfo), 0);
            instance->m_ComputeEncoder->setTexture(texture, 0);
            instance->m_ComputeEncoder->setTexture(texture, 1);
        });
        
        DispatchCompute({ blitInfo.DstExtent.Width, blitInfo.DstExtent.Height, blitInfo.DstExtent.Depth });
    }

    void MetalRenderCommandBuffer::ImageMemoryBarrier(const ImageMemoryInfo& imageMemory)
    {
        Ref<MetalRenderCommandBuffer> instance = this;
        Renderer::Submit([instance, imageMemory]() mutable {
            auto encoder = instance->m_ComputeEncoder;
            encoder->memoryBarrier(MTL::BarrierScopeTextures);
        });
    }
    void MetalRenderCommandBuffer::DispatchCompute(GroupSize GlobalGroupSize)
    {
        m_WaitOnSubmit = true;
        Ref<MetalRenderCommandBuffer> instance = this;
        
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
}
#endif

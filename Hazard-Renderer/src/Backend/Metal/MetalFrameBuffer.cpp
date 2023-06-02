

#include "MetalFrameBuffer.h"
#ifdef HZR_INCLUDE_METAL
#include "MetalContext.h"
#include "MetalSwapchain.h"
#include "MetalRenderPass.h"

#include "spdlog/fmt/fmt.h"

#include "Backend/Core/Renderer.h"

namespace HazardRenderer::Metal
{
    MetalFrameBuffer::MetalFrameBuffer(FrameBufferCreateInfo* info)
    {
        m_Specs.DebugName = info->DebugName;
        m_Specs.AttachmentCount = info->AttachmentCount;
        m_Specs.ClearColor = info->ClearColor;
        m_Specs.Attachments = info->Attachments;
        m_Specs.pFrameBuffer = info->pFrameBuffer;
        m_Specs.SwapChainTarget = info->SwapChainTarget;
        m_Specs.ClearColor = info->ClearColor;
        m_Specs.Samples = info->Samples;
        
        if(info->Width == 0)
        {
            //Swapchain size
            auto swapchain = MetalContext::GetInstance()->GetSwapchain();
            m_Specs.Width = swapchain->GetWidth();
            m_Specs.Height = swapchain->GetHeight();
        }
        else
        {
            m_Specs.Width = info->Width;
            m_Specs.Height = info->Height;
        }
        
        m_RenderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();
        
        if(!m_Specs.pFrameBuffer)
        {
            uint32_t attachmentIndex = 0;
            
            for (auto& attachmentSpec : m_Specs.Attachments)
            {
                if (m_ExistingImage && m_ExistingImage->GetLayerCount() > 1)
                {
                    if (attachmentSpec.IsDepth())
                        m_DepthAttachmentImage = m_ExistingImage;
                    else m_ColorAttachments.emplace_back(m_ExistingImage);
                }
                else if (m_ExistingImages.find(attachmentIndex) != m_ExistingImages.end())
                {
                    if (!attachmentSpec.IsDepth())
                        m_ColorAttachments.emplace_back();
                }
                else if (attachmentSpec.IsDepth())
                {
                    Image2DCreateInfo imageInfo = {};
                    imageInfo.Format = attachmentSpec.Format;
                    imageInfo.Usage = ImageUsage::Attachment;
                    imageInfo.Extent.Width = m_Specs.Width;
                    imageInfo.Extent.Height = m_Specs.Height;
                    imageInfo.DebugName = fmt::format("{0}-Depth Attachment {1}", m_Specs.DebugName.empty() ? "Unnamed FB" : m_Specs.DebugName, attachmentIndex);
                    m_DepthAttachmentImage = Image2D::Create(&imageInfo).As<MetalImage2D>();
                }
                else
                {
                    Image2DCreateInfo imageInfo = {};
                    imageInfo.Format = attachmentSpec.Format;
                    imageInfo.Usage = ImageUsage::Attachment;
                    imageInfo.Extent.Width = m_Specs.Width;
                    imageInfo.Extent.Height = m_Specs.Height;
                    imageInfo.DebugName = fmt::format("{0}-Color Attachment {1}", m_Specs.DebugName.empty() ? "Unnamed FB" : m_Specs.DebugName, attachmentIndex);
                    m_ColorAttachments.emplace_back(Image2D::Create(&imageInfo).As<MetalImage2D>());
                }
                attachmentIndex++;
            }
        }
        
        Resize(m_Specs.Width, m_Specs.Height, true);
    }
    MetalFrameBuffer::~MetalFrameBuffer()
    {
        m_RenderPassDescriptor->release();
    }
    void MetalFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
    {
        HZR_PROFILE_FUNCTION();
        Ref<MetalFrameBuffer> instance = this;
        Renderer::SubmitResourceCreate([instance, width, height, force]() mutable {
            instance->Resize_RT(width, height, force);
            });
    }
    void MetalFrameBuffer::Resize_RT(uint32_t width, uint32_t height, bool force)
    {
        HZR_PROFILE_FUNCTION();
        if (!force && (m_Specs.Width == width && m_Specs.Height == height)) return;

        m_Specs.Width = width;
        m_Specs.Height = height;

        if (!m_Specs.SwapChainTarget)
        {
            Invalidate_RT();
        }
        else
        {
            auto swapchain = MetalContext::GetInstance()->GetSwapchain();
            
            m_ClearValues.clear();
            m_ClearValues.push_back(MetalContext::GetInstance()->GetClearColorValue());
        }
    }
    void MetalFrameBuffer::Release()
    {
        
    }
    void MetalFrameBuffer::Release_RT()
    {
        
    }
    void MetalFrameBuffer::Invalidate()
    {
        HZR_PROFILE_FUNCTION();
        Ref<MetalFrameBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    void MetalFrameBuffer::Invalidate_RT()
    {
        const auto device = MetalContext::GetMetalDevice();
        
        Release_RT();
        
        m_ClearValues.resize(m_Specs.AttachmentCount);
        
        if(m_Specs.pFrameBuffer)
            m_ColorAttachments.clear();
        
        //bool createImages = m_ColorAttachments.empty();

        uint32_t attachmentImageIndex = 0;
        for(auto& spec : m_Specs.Attachments)
        {
            //Check depth image
            if(spec.IsDepth())
            {
                if(m_ExistingImage)
                {
                    m_DepthAttachmentImage = m_ExistingImage;
                }
                else if(m_Specs.pFrameBuffer)
                {
                    Ref<MetalFrameBuffer> target = m_Specs.pFrameBuffer.As<MetalFrameBuffer>();
                    m_DepthAttachmentImage = target->GetDepthImage().As<MetalImage2D>();
                }
                else if (m_ExistingImages.find(attachmentImageIndex) != m_ExistingImages.end())
                {
                    Ref<MetalImage2D> existingImage = m_ExistingImages[attachmentImageIndex];
                    HZR_ASSERT(existingImage->GetFormat() >= ImageFormat::DEPTH32F, "Color image cannot be depth image");
                    m_DepthAttachmentImage = existingImage;
                }
                else
                {
                    m_DepthAttachmentImage->Resize_RT(m_Specs.Width, m_Specs.Height);
                }
                
                MTL::RenderPassDepthAttachmentDescriptor* depthDescriptor = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();
                
                depthDescriptor->setClearDepth(1.0f);
                depthDescriptor->setLevel(0);
                depthDescriptor->setLoadAction(m_Specs.ClearOnLoad ? MTL::LoadActionClear : MTL::LoadActionLoad);
                depthDescriptor->setStoreAction(MTL::StoreActionStore);
                depthDescriptor->setTexture(m_DepthAttachmentImage->GetMetalTexture());
            }
            else //Color attachment
            {
                
            }
        }
        
    }
}
#endif

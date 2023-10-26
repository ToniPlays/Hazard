
#include "EditorPlatformMetal.h"

#ifdef HZR_INCLUDE_METAL
#include "Core/Renderer.h"
#include "Metal/MetalFrameBuffer.h"
#include "Metal/MetalRenderCommandBuffer.h"
#include "Metal/MetalSwapchain.h"

#include "ImGui_Backend/imgui_impl_glfw.h"
#include "ImGui_Backend/imgui_impl_metal.h"

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

#import <Metal/Metal.hpp>
#include <GLFW/glfw3.h>

EditorPlatformMetal::EditorPlatformMetal(HazardRenderer::Window& window)
{
    using namespace HazardRenderer::Metal;
    m_Window = &window;
    m_Context = (MetalContext*)window.GetContext();
    
    Ref<MetalPhysicalDevice> device = m_Context->GetDevice();
    
    m_Descriptor = MTL::RenderPassDescriptor::alloc()->init();
    m_Descriptor->setDefaultRasterSampleCount(1);
    m_Descriptor->setRenderTargetWidth(window.GetWidth() * window.GetWindowInfo().FramebufferScale.x);
    m_Descriptor->setRenderTargetWidth(window.GetHeight() * window.GetWindowInfo().FramebufferScale.y);
    
    MTL::RenderPassColorAttachmentDescriptor* attachment = m_Descriptor->colorAttachments()->object(0);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setStoreAction(MTL::StoreActionStore);
    
    ImGui_ImplMetal_Init((__bridge id<MTLDevice>)(device->GetMetalDevice()));
}

EditorPlatformMetal::~EditorPlatformMetal()
{
#ifdef HZR_PLATFORM_MACOS
    ImGui_ImplGlfw_Shutdown();
#endif
    ImGui_ImplMetal_Shutdown();
}

void EditorPlatformMetal::BeginFrame()
{
    using namespace HazardRenderer::Metal;
    
    auto swapchain = MetalContext::GetInstance()->GetSwapchain().As<MetalSwapchain>();
    auto drawable = swapchain->GetDrawable();
    auto* colorAttachment = m_Descriptor->colorAttachments()->object(0);
    colorAttachment->setTexture(drawable->texture());
    
    ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)(m_Descriptor));
#ifdef HZR_PLATFORM_MACOS
    ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();
}

void EditorPlatformMetal::EndFrame()
{
    using namespace HazardRenderer::Metal;
    auto swapchain = MetalContext::GetInstance()->GetSwapchain();
    auto cmdBuffer = swapchain->GetSwapchainBuffer().As<MetalRenderCommandBuffer>();
    
    uint32_t w = m_Context->GetSwapchain()->GetWidth();
    uint32_t h = m_Context->GetSwapchain()->GetHeight();
    
    m_Descriptor->setRenderTargetWidth(w);
    m_Descriptor->setRenderTargetHeight(h);
    
    auto encoder = cmdBuffer->GetMetalCommandBuffer()->renderCommandEncoder(m_Descriptor);
    
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), (__bridge id<MTLCommandBuffer>)(cmdBuffer->GetMetalCommandBuffer()), (__bridge id<MTLRenderCommandEncoder>)encoder);
    
    encoder->endEncoding();
}

void EditorPlatformMetal::Close()
{
    ImGui::DestroyContext();
}

#endif

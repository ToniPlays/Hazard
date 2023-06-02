
#include "EditorPlatformMetal.h"
#ifdef HZR_INCLUDE_METAL
#include "Backend/Core/Renderer.h"
#include "Backend/Metal/MetalFrameBuffer.h"
#include "Backend/Metal/MetalRenderCommandBuffer.h"
#include "Backend/Metal/MetalSwapchain.h"

//#include "../ImGui_Backend/imgui_impl_metal.h"
#include "../ImGui_Backend/imgui_impl_glfw.h"

#import <Metal/Metal.h>
#include <GLFW/glfw3.h>

EditorPlatformMetal::EditorPlatformMetal(HazardRenderer::Window& window)
{
    using namespace HazardRenderer::Metal;
    /*
    m_Window = &window;
    m_Context = (MetalContext*)window.GetContext();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.GetNativeWindow(), true);
    
    auto device = MetalContext::GetMetalDevice();
    
    ImGui_ImplMetal_Init((__bridge id<MTLDevice>)(device->GetMetalDevice()));
    
    m_Descriptor = MTL::RenderPassDescriptor::alloc()->init();
    m_Descriptor->setDefaultRasterSampleCount(1);
    m_Descriptor->setRenderTargetWidth(window.GetWidth());
    m_Descriptor->setRenderTargetWidth(window.GetHeight());
    
    auto* attachment = m_Descriptor->colorAttachments()->object(0);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setStoreAction(MTL::StoreActionStore);
    
    ImGuiIO& io = ImGui::GetIO();
    
    float xScale, yScale;
    glfwGetWindowContentScale((GLFWwindow*)m_Window->GetNativeWindow(), &xScale, &yScale);
    io.DisplayFramebufferScale = { xScale, yScale };
     */
}

EditorPlatformMetal::~EditorPlatformMetal()
{
    ImGui_ImplGlfw_Shutdown();
    //ImGui_ImplMetal_Shutdown();
}

void EditorPlatformMetal::BeginFrame()
{
    using namespace HazardRenderer::Metal;
    /*
    auto swapchain = m_Context->GetSwapchain();
    
    uint32_t w = swapchain->GetWidth();
    uint32_t h = swapchain->GetHeight();
    
    m_Descriptor->setRenderTargetWidth(w);
    m_Descriptor->setRenderTargetHeight(h);
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = { (float)w, (float)h };
    
    CA::MetalDrawable* drawable = swapchain.As<MetalSwapchain>()->GetDrawable();
    m_Descriptor->colorAttachments()->object(0)->setTexture(drawable->texture());
    
    ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)(m_Descriptor));
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    */
}

void EditorPlatformMetal::EndFrame()
{
    /*
    using namespace HazardRenderer::Metal;
    auto swapchain = m_Context->GetSwapchain();
    auto cmdBuffer = swapchain->GetSwapchainBuffer().As<MetalRenderCommandBuffer>();
    cmdBuffer->BeginRenderPass_RT(swapchain->GetRenderPass());
    
           
    id<MTLCommandBuffer> buffer = (__bridge id<MTLCommandBuffer>)(cmdBuffer->GetMetalCommandBuffer());
    id<MTLRenderCommandEncoder> encoder = (__bridge id<MTLRenderCommandEncoder>)(cmdBuffer->GetRenderEncoder());
           
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), buffer, encoder);
    cmdBuffer->GetRenderEncoder()->endEncoding();
     */
}

void EditorPlatformMetal::Close()
{
    ImGui::DestroyContext();
}
#endif

#include "TriangleTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "File.h"

#include <spdlog/fmt/fmt.h>

void TriangleTest::Reset()
{
    
}

void TriangleTest::Init()
{
    using namespace HazardRenderer;
    m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();
    
    m_Window->GetContext()->SetClearColor(Color(255, 128, 0, 255));
    m_Window->SetWindowTitle(GetName());
    
    std::cout << "Selected device: " << m_Window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
    
    std::unordered_map<RenderAPI, std::string> extensions = {
        { RenderAPI::OpenGL, "ogl" },
        { RenderAPI::Vulkan, "vk" },
        { RenderAPI::Metal, "mtl" }
    };
    
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f
    };
    
    BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
        { "a_Color",	ShaderDataType::Float4 }
    };
    
    BufferCreateInfo vbo = {};
    vbo.Name = "TriangleVBO";
    vbo.Size = sizeof(vertices);
    vbo.Data = vertices;
    vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;
    
    DescriptorSetLayout descriptorLayout = {};
    
    PipelineSpecification spec = {};
    spec.DebugName = "Pipeline";
    spec.Usage = PipelineUsage::GraphicsBit;
    spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
    spec.pBufferLayout = &layout;
    spec.Flags = PIPELINE_DRAW_FILL | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    spec.Shaders = { { SHADER_STAGE_VERTEX_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/triangle.Vertex.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) },
                     { SHADER_STAGE_FRAGMENT_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/triangle.Fragment.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) } };
    
    m_VertexBuffer = GPUBuffer::Create(&vbo);
    m_Pipeline = Pipeline::Create(&spec);
}

void TriangleTest::Run()
{
    auto swapchain = m_Window->GetSwapchain();
    auto commandBuffer = swapchain->GetSwapchainBuffer();
    
    commandBuffer->BeginRenderPass(swapchain->GetRenderPass());
    commandBuffer->SetPipeline(m_Pipeline);
    commandBuffer->SetVertexBuffer(m_VertexBuffer);
    commandBuffer->Draw(6);
    commandBuffer->EndRenderPass();
}

void TriangleTest::Terminate()
{
    
}

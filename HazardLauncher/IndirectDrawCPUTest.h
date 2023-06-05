
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include <filesystem>

#include <glad/glad.h>

using namespace HazardRenderer;

namespace IndirectDrawCPUTest {

    //OpenGL: Test
    //Vulkan: Test
    //Metal : Test
    //DX12  : Test
    //DX11  : Test

    static void Run(RenderAPI api)
    {
        static bool running = true;
        
        //Window creation
        HazardRendererAppInfo appInfo = {};
        appInfo.AppName = "Indirect draw on CPU";
        appInfo.BuildVersion = "0.0.1a";
        appInfo.MessageCallback = [](RenderMessage message) {
            std::cout << message.Description << std::endl;
        };
        appInfo.EventCallback = [](Event& e) {
            EventDispatcher dispatcher(e);
            if (e.GetEventType() == EventType::WindowClose) {
                running = false;
            }
        };

        HazardRendererAppInfo rendererApp = {};
        rendererApp.AppName = appInfo.AppName;
        rendererApp.BuildVersion = "1.0.0!";
        rendererApp.EventCallback = appInfo.EventCallback;

        rendererApp.MessageCallback = [](RenderMessage message)
        {
            std::cout << message.Description << std::endl;
            std::cout << message.StackTrace << std::endl;
        };

        HazardWindowCreateInfo windowInfo = {};
        windowInfo.Title = appInfo.AppName;
        windowInfo.Extent = { 1920, 1080 };
        windowInfo.Color = Color(255, 128, 0, 255);

        HazardRendererCreateInfo renderInfo = {};
        renderInfo.pAppInfo = &rendererApp;
        renderInfo.Renderer = api;
        renderInfo.Logging = true;
        renderInfo.VSync = true;
        renderInfo.WindowCount = 1;
        renderInfo.pWindows = &windowInfo;

        Window* window = Window::Create(&renderInfo);
        window->Show();
        //---------------
        
        std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
        float vertices[] =
        {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout layout = { { "a_Position",            ShaderDataType::Float3 },
                                { "a_Color",            ShaderDataType::Float4 },
                                { "a_TextureCoords",    ShaderDataType::Float2 }
        };

        std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/texturedQuad.glsl", api);

        stbi_set_flip_vertically_on_load(true);

        int w, h, channels;
        stbi_uc* data = stbi_load("assets/textures/csharp.png", &w, &h, &channels, 4);

        VertexBufferCreateInfo vbo = {};
        vbo.Name = "TriangleVBO";
        vbo.Layout = &layout;
        vbo.Size = sizeof(vertices);
        vbo.Data = &vertices;

        IndexBufferCreateInfo ibo = {};
        ibo.Name = "TriangleIBO";
        ibo.Size = sizeof(indices);
        ibo.Data = indices;

        PipelineSpecification spec = {};
        spec.DebugName = "Pipeline";
        spec.Usage = PipelineUsage::GraphicsBit;
        spec.DrawType = DrawType::Fill;
        spec.CullMode = CullMode::None;
        spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
        spec.DepthTest = false;
        spec.pBufferLayout = &layout;
        spec.ShaderCodeCount = code.size();
        spec.pShaderCode = code.data();

        Image2DCreateInfo imageInfo = {};
        imageInfo.DebugName = "Image2D";
        imageInfo.Extent = { (uint32_t)w, (uint32_t)h, 1 };
        imageInfo.Format = ImageFormat::RGBA;
        imageInfo.Data = Buffer(data, w * h * 4);
        imageInfo.Usage = ImageUsage::Texture;

        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
        Ref<Pipeline> pipeline = Pipeline::Create(&spec);
        Ref<Image2D> image = Image2D::Create(&imageInfo);

        pipeline->GetShader()->Set("u_Texture", 0, image);
        
        while (running)
        {
            auto swapchain = window->GetSwapchain();
            auto commandBuffer = swapchain->GetSwapchainBuffer();
            auto renderPass = swapchain->GetRenderPass();
            
            Input::Update();
            window->BeginFrame();
            
            commandBuffer->BeginRenderPass(renderPass);
            commandBuffer->SetVertexBuffer(vertexBuffer);
            commandBuffer->SetPipeline(pipeline);
            commandBuffer->DrawIndirect(1, indexBuffer);
            commandBuffer->EndRenderPass();
            
            Renderer::WaitAndRender();
            window->Present();
        }
    }
}


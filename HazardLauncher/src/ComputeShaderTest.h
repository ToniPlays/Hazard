
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include <filesystem>

#include <glad/glad.h>

using namespace HazardRenderer;

namespace ComputeShaderTest {

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
        appInfo.AppName = "Compute shader";
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
       
        BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
                                { "a_Color", ShaderDataType::Float4 },
                                { "a_TextureCoord", ShaderDataType::Float2 }
        };
        
        float vertices[] =
        {
            -0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
        };
        
        VertexBufferCreateInfo vbo = {};
        vbo.Name = "TriangleVBO";
        vbo.Layout = &layout;
        vbo.Size = sizeof(vertices);
        vbo.Data = &vertices;
        
        std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/texturedQuad.glsl", api);
        
        PipelineSpecification spec = {};
        spec.DebugName = "Rasterized";
        spec.Usage = PipelineUsage::GraphicsBit;
        spec.DrawType = DrawType::Fill;
        spec.CullMode = CullMode::None;
        spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
        spec.DepthTest = false;
        spec.pBufferLayout = &layout;
        spec.ShaderCodeCount = code.size();
        spec.pShaderCode = code.data();
        
        std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/compute.glsl", api);
        
        PipelineSpecification computeSpec = {};
        computeSpec.DebugName = "Compute";
        computeSpec.Usage = PipelineUsage::ComputeBit;
        computeSpec.ShaderCodeCount = computeCode.size();
        computeSpec.pShaderCode = computeCode.data();
        
        Image2DCreateInfo outputImageSpec = {};
        outputImageSpec.DebugName = "ComputeOutput";
        outputImageSpec.Extent = { 512, 512, 1 };
        outputImageSpec.Mips = 1;
        outputImageSpec.Format = ImageFormat::RGBA;
        outputImageSpec.Usage = ImageUsage::Storage;
        
        
        Ref<Image2D> outputImage = Image2D::Create(&outputImageSpec);
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
        Ref<Pipeline> pipeline = Pipeline::Create(&spec);
        Ref<Pipeline> compute = Pipeline::Create(&computeSpec);
        Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Compute", 1, true);
        
        //Compute step
        {
            DispatchComputeInfo info = {};
            info.WaitForCompletion = true;
            info.GroupSize = { 512, 512, 1 };
            
            compute->GetShader()->Set("o_OutputImage", 0, outputImage);
            computeBuffer->Begin();
            computeBuffer->SetPipeline(compute);
            computeBuffer->DispatchCompute(info);
            computeBuffer->End();
            computeBuffer->Submit();
        }
        
        pipeline->GetShader()->Set("u_Texture", 0, outputImage);

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
            commandBuffer->Draw(6);
            commandBuffer->EndRenderPass();
            
            Renderer::WaitAndRender();
            window->Present();
        }
    }
}


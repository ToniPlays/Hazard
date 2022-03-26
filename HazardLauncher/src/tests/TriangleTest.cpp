
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

using namespace HazardUtility;
using namespace HazardRenderer;

namespace TriangleTest {

	static void Run()
	{
        std::cout << "Running triangle test" << std::endl;
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello Triangle";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};
		appInfo.EventCallback = [](Event& e) {
			EventDispatcher dispatcher(e);
			if (e.GetEventType() == EventType::WindowClose) {
				running = false;
			}
		};
        
		HazardRendererCreateInfo createInfo = {};
#ifdef HZR_PLATFORM_WINDOWS
		createInfo.Renderer = RenderAPI::Vulkan;
#elif HZR_PLATFORM_MACOS
        createInfo.Renderer = RenderAPI::Metal;
#endif
		createInfo.Width = 1280;
		createInfo.Height = 720;
		createInfo.VSync = true;
		createInfo.Logging = true;
		createInfo.Color = { 0.1f, 0.1f, 0.125f, 1.0f };
		createInfo.AppInfo = &appInfo;
      
        
		Window* window = Window::Create(&createInfo);
        window->Show();
        
        std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;
        
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2
		};

		BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
								{ "a_Color", ShaderDataType::Float4 }
		};
        
		VertexBufferCreateInfo vbo = {};
		vbo.DebugName = "TriangleVBO";
		vbo.Usage = BufferUsage::StaticDraw;
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "TriangleIBO";
		ibo.Usage = BufferUsage::StaticDraw;
		ibo.Size = 3 * sizeof(uint32_t);
		ibo.Data = indices;
        

		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "ScreenFBO";
		frameBufferInfo.SwapChainTarget = true;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };

		Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(&frameBufferInfo);
		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "ScreenTarget";
		renderPassInfo.pTargetFrameBuffer = frameBuffer;

		Ref<RenderPass> renderPass = RenderPass::Create(&renderPassInfo);

		PipelineSpecification spec = {};
		spec.DebugName = "Pipeline";
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.DrawType = DrawType::Fill;
		spec.ShaderPath = "triangleShader.glsl";
		spec.TargetRenderPass = renderPass;

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::CreateFromSwapchain("Main");

		while (running)
		{
			window->BeginFrame();

			cmdBuffer->Begin();
			window->GetContext()->BeginRenderPass(cmdBuffer, renderPass);

			vertexBuffer->Bind(cmdBuffer);
			indexBuffer->Bind(cmdBuffer);
			pipeline->Bind(cmdBuffer);
			pipeline->Draw(cmdBuffer, 3);

			window->GetContext()->EndRenderPass(cmdBuffer);
			cmdBuffer->End();

			window->Present();
		}
         
        std::cout << "Test closed";
	}
}

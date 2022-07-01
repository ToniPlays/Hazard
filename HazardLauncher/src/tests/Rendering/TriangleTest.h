
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include <glad/glad.h>

using namespace HazardRenderer;

namespace TriangleTest {

	//OpenGL: Pass
	//Vulkan: Pass
	//Metal: Pass with memory leak 

	static void Run(RenderAPI api)
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

		HazardRendererAppInfo rendererApp = {};
		rendererApp.AppName = appInfo.AppName;
		rendererApp.BuildVersion = "1.0.0!";
		rendererApp.EventCallback = appInfo.EventCallback;

		rendererApp.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};


		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = "HazardEditor";
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Decorated = true;
		windowInfo.Width = 1280;
		windowInfo.Height = 720;
		windowInfo.Color = Color(255, 128, 0, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;
      
        
		Window* window = Window::Create(&renderInfo);
        window->Show();

		//---------------
#pragma region Mesh
        std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;
        
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f
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
		ibo.Size = sizeof(indices);
		ibo.Data = indices;
        

		PipelineSpecification spec = {};
		spec.DebugName = "Pipeline";
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.DrawType = DrawType::Fill;
#ifdef HZR_PLATFORM_WINDOWS
		spec.ShaderPath = "triangleShader.glsl";
#elif HZR_PLATFORM_MACOS
        spec.ShaderPath = "res/triangleShader.metal";
#endif
		spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
		spec.DepthTest = false;
		spec.pBufferLayout = &layout;

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
		Ref<Pipeline> pipeline = Pipeline::Create(&spec);
		
#pragma endregion
		while (running)
		{
			Ref<RenderCommandBuffer> cmdBuffer = window->GetSwapchain()->GetSwapchainBuffer();
			window->BeginFrame();

			vertexBuffer->Bind(cmdBuffer);
			indexBuffer->Bind(cmdBuffer);

			pipeline->Bind(cmdBuffer);
			pipeline->Draw(cmdBuffer, indexBuffer->GetCount());

			Renderer::WaitAndRender();
			window->Present();
		}

		pipeline.Release();
		indexBuffer.Release();
		vertexBuffer.Release();

		window->Close();

		delete window;
         
        std::cout << "Test closed";
	}
}

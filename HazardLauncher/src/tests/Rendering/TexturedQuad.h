
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

using namespace HazardRenderer;

namespace TexturedQuad {

	//OpenGL: Nothing
	//Vulkan: White quad
	//Metal: Not tested

	static void Run(RenderAPI api)
	{
		std::cout << "Running textured quad test" << std::endl;
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello Textured quad";
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

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = "Textured quad " + RenderAPIToString(api);
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Decorated = true;
		windowInfo.Width = 1920;
		windowInfo.Height = 1080;
		windowInfo.Color = Color(34, 34, 34, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &appInfo;
		renderInfo.Renderer = api;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;


		Window* window = Window::Create(&renderInfo);
		window->Show();

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
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
		spec.ShaderPath = "triangleShader.glsl";
		spec.TargetRenderPass = window->GetSwapchain()->GetRenderPass();
		spec.pBufferLayout = &layout;

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		while (running)
		{
			window->BeginFrame();

			auto& cmdBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			vertexBuffer->Bind(cmdBuffer);
			indexBuffer->Bind(cmdBuffer);
			pipeline->Bind(cmdBuffer);
			pipeline->Draw(cmdBuffer, indexBuffer->GetCount());

			window->Present();
		}

		std::cout << "Test closed";
	}
}

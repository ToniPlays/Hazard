
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;

namespace AccelerationStructureTest
{

	//OpenGL : Test
	//Vulkan : Test
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
#ifdef HZR_PLATFORM_MACOS
		std::filesystem::current_path("/users/ToniSimoska/Hazard/HazardLauncher");
#endif
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello Acceleration structures";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.MessageCallback = [](RenderMessage message) 
		{
			std::cout << message.Description << std::endl;
		};
		appInfo.EventCallback = [](Event& e) {
			EventDispatcher dispatcher(e);
			if (e.GetEventType() == EventType::WindowClose) 
			{
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
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Width = 1280;
		windowInfo.Height = 720;
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

		auto& device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

		//---------------
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

		BufferLayout layout = { { "a_Position",			ShaderDataType::Float3 },
								{ "a_Color",			ShaderDataType::Float4 },
								{ "a_TextureCoords",	ShaderDataType::Float2 }
		};

		VertexBufferCreateInfo vbo = {};
		vbo.DebugName = "QuadVBO";
		vbo.Usage = BufferUsage::BLAS;
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "QuadIBO";
		ibo.Usage = BufferUsage::BLAS;
		ibo.Size = sizeof(indices);
		ibo.Data = indices;

		BoundingBox boundingBox = {};
		boundingBox.Encapsulate({-0.5f, -0.5f, 0.0f });
		boundingBox.Encapsulate({ 0.5f, -0.5f, 0.0f });
		boundingBox.Encapsulate({ 0.5f,  0.5f, 0.0f });
		boundingBox.Encapsulate({-0.5f,  0.5f, 0.0f });

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.Level = AccelerationStructureLevel::Top;

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.VertexBuffer = vertexBuffer;
		bottomAccelInfo.IndexBuffer = indexBuffer;
		bottomAccelInfo.BoundingBox = boundingBox;



		Ref<AccelerationStructure> topLevelAccelerationStructure = AccelerationStructure::Create(&topAccelInfo);
		Ref<AccelerationStructure> bottomLevelAccelerationStructure = AccelerationStructure::Create(&bottomAccelInfo);

		while (running)
		{
			Input::Update();
			window->BeginFrame();

			auto& commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());

			commandBuffer->EndRenderPass();
			Renderer::WaitAndRender();
			window->Present();
		}

		std::cout << "Test closed";
	}
}

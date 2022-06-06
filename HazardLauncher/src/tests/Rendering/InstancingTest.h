
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "MathCore.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GLFW/glfw3.h"
#include <glad/glad.h>

using namespace HazardRenderer;

namespace InstancingTest {

	//OpenGL: 
	//Vulkan: Black window
	//Metal:  

	static void Run(RenderAPI api)
	{
		uint32_t size = 5;

		std::cout << "Running instancing test with " << size * size << " quads" << std::endl;
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello Instancing";
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
		rendererApp.BuildVersion = "1.0.0";
		rendererApp.EventCallback = [&](Event& e) {

		};
		rendererApp.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = appInfo.AppName;
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Decorated = true;
		windowInfo.Width = 1920;
		windowInfo.Height = 1080;
		windowInfo.Color = Color(34, 34, 34, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.VSync = false;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();
		window->SetWindowTitle(window->GetWindowInfo().Title + " quads: " + std::to_string(size * size));

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		BufferLayout instanceLayout = {
			{ "a_Position", ShaderDataType::Float3                      },
			{ "a_Color",    ShaderDataType::Float4, false, PerInstance  },
			{ "a_MRo0",     ShaderDataType::Float4, false, PerInstance  },
			{ "a_MRo1",     ShaderDataType::Float4, false, PerInstance  },
			{ "a_MRo2",     ShaderDataType::Float4, false, PerInstance  }
		};

		struct InstanceData {
			TransformData transform;
		};

		VertexBufferCreateInfo vbo = {};
		vbo.DebugName = "InstancedQuad";
		vbo.IsShared = false;
		vbo.Layout = &instanceLayout;
		vbo.Size = sizeof(vertices);
		vbo.Data = vertices;

		Ref<VertexBuffer> quadBuffer = VertexBuffer::Create(&vbo);

		VertexBufferCreateInfo instanceVBO = {};
		instanceVBO.DebugName = "InstanceVBO";
		instanceVBO.Usage = BufferUsage::DynamicDraw;
		instanceVBO.Size = size * size * sizeof(InstanceData);

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "TriangleIBO";
		ibo.Usage = BufferUsage::StaticDraw;
		ibo.Size = sizeof(indices);
		ibo.Data = indices;

		Ref<VertexBuffer> instanceBuffer = VertexBuffer::Create(&instanceVBO);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

		PipelineSpecification spec = {};
		spec.DebugName = "InstancePipeline";
		spec.ShaderPath = "QuadInstanced.glsl";
		spec.DrawType = DrawType::Fill;
		spec.CullMode = CullMode::None;
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.TargetRenderPass = window->GetSwapchain()->GetRenderPass();
		spec.pBufferLayout = &instanceLayout;

		Ref<Pipeline> pipeline = Pipeline::Create(&spec);


		float scalar = 10.0f;
		float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
		glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
		glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);

		Camera camera = {};
		camera.SetProjection(projection);
		camera.SetView(view);


		InstanceData* instanceData = new InstanceData[size * size];

		float halfSize = (float)size / 2.0f;

		uint32_t index = 0;

		for (uint32_t x = 0; x < size; x++) {
			for (uint32_t y = 0; y < size; y++) {

				InstanceData& data = instanceData[index];
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x - halfSize, y - halfSize, 0 });

				data.transform.Color = glm::vec4((float)x / (float)size, (float)y / (float)size, 0.0f, 1.0f);

				data.transform.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
				data.transform.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
				data.transform.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

				index++;
			}
		}

		instanceBuffer->SetData(instanceData, size * size * sizeof(InstanceData));

		double startTime = 0;

		std::string title = window->GetWindowInfo().Title;
		pipeline->GetShader()->SetUniformBuffer("Camera", &camera.GetViewPprojection(), sizeof(Camera));

		while (running)
		{
			Ref<RenderCommandBuffer> cmdBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			double time = glfwGetTime();
			window->SetWindowTitle(title + " frame time " + std::to_string((time - startTime) * 1000.0f));
			startTime = time;

			window->BeginFrame();

			quadBuffer->Bind(cmdBuffer);
			instanceBuffer->Bind(cmdBuffer, 1);
			indexBuffer->Bind(cmdBuffer);
			pipeline->Bind(cmdBuffer);
			pipeline->DrawInstanced(cmdBuffer, indexBuffer->GetCount(), size * size);

			window->Present();
		}

		std::cout << "Test closed";
	}
}

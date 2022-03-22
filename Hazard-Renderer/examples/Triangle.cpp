
#include "HazardRenderer.h"
#include "CommandQueue.h"

#include <iostream>

namespace Triangle 
{
	using namespace HazardRenderer;
	using namespace HazardUtility;
	//OpenGL: Running

	static void OnDebugInfo(RenderMessage message) {
		std::cout << message.Message << std::endl;
	}

	static void Run()
	{
		try {
			std::vector<const char*> icons = { "res/world.png", "res/world.png" };

			HazardRendererAppInfo appInfo = {};
			appInfo.AppName = "Example triangle";
			appInfo.BuildVersion = 1;
			appInfo.Icons = icons.data();
			appInfo.IconCount = 2;
			appInfo.MessageCallback = OnDebugInfo;

			HazardRendererCreateInfo createInfo = {};
			createInfo.Renderer = RenderAPI::Vulkan;
			createInfo.Width = 1280;
			createInfo.Height = 720;
			createInfo.Maximized = false;
			createInfo.FullScreen = false;
			createInfo.VSync = false;
			createInfo.Color = { 0.2f, 0.2f, 0.2f, 1.0f };
			createInfo.AppInfo = &appInfo;

			Window* window = Window::Create(&createInfo);
			window->Show();

			std::cout << "Selected device " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

			float vertices[] = {
				-0.75f, -0.75f, 0.0f,
				 0.00f,  0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f
			};
			uint32_t indices[] = { 0, 1, 2 };

			BufferLayout layout = { { "v_Position", ShaderDataType::Float3 } };

			VertexBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.DebugName = "TriangleVertexBuffer";
			vertexBufferInfo.IsShared = false;
			vertexBufferInfo.Size = 9 * sizeof(float);
			vertexBufferInfo.Data = &vertices;
			vertexBufferInfo.Layout = &layout;

			IndexBufferCreateInfo indexBufferInfo = {};
			indexBufferInfo.DebugName = "TriangleIndexBuffer";
			indexBufferInfo.IsShared = false;
			indexBufferInfo.Size = 3;
			indexBufferInfo.Usage = BufferUsage::StaticDraw;
			indexBufferInfo.Data = indices;

			PipelineSpecification pipelineSpec = {};
			pipelineSpec.DebugName = "TrianglePipeline";
			pipelineSpec.DrawType = DrawType::Fill;
			pipelineSpec.ShaderPath = "standard.glsl";
			pipelineSpec.DepthTest = false;
			pipelineSpec.CullMode = CullMode::None;


			Ref<VertexBuffer> triangleVertexBuffer = VertexBuffer::Create(&vertexBufferInfo);
			Ref<IndexBuffer> triangleIndexBuffer = IndexBuffer::Create(&indexBufferInfo);
			Ref<Pipeline> pipeline = Pipeline::Create(&pipelineSpec);

			Ref<RenderCommandBuffer> commandBuffer = RenderCommandBuffer::CreateFromSwapchain("Main");
			CommandQueue* queue = new CommandQueue(10 * 1024);

			queue->Submit([triangleVertexBuffer, triangleIndexBuffer, pipeline, commandBuffer]() mutable {
				triangleVertexBuffer->Bind(commandBuffer);
				triangleIndexBuffer->Bind(commandBuffer);
				pipeline->Bind(commandBuffer);
				pipeline->Draw(commandBuffer, triangleIndexBuffer->GetCount());
				});

			while (true)
			{
				window->BeginFrame();
				commandBuffer->Begin();

				queue->Excecute();

				commandBuffer->End();
				commandBuffer->Submit();

				window->Present();
			}
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}
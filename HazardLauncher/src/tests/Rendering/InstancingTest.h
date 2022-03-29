
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "MathCore.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glad/glad.h>

using namespace HazardUtility;
using namespace HazardRenderer;

namespace InstancingTest {

	//OpenGL: 
	//Vulkan: Black window
	//Metal:  

	static void Run(RenderAPI api)
	{
		uint32_t size = 500;

		std::cout << "Running instancing test" << std::endl;
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

		createInfo.Renderer = api;
		createInfo.Width = 1280;
		createInfo.Height = 720;
		createInfo.VSync = false;
		createInfo.Logging = true;
		createInfo.Color = { 0.1f, 0.1f, 0.125f, 1.0f };
		createInfo.AppInfo = &appInfo;

		Window* window = Window::Create(&createInfo);
		window->Show();

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		float vertices[] =
		{
			-0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		BufferLayout instanceLayout = { { "a_Position", ShaderDataType::Float3 }, { "a_Color", ShaderDataType::Float4 } };
		//BufferLayout perInstanceLayout = { { { "a_MRow0", ShaderDataType::Float4 }, { "a_MRow1", ShaderDataType::Float4 }, { "a_MRow2", ShaderDataType::Float4 } }, PerInstance };
		BufferLayout perInstanceLayout = { { { "a_Offset", ShaderDataType::Float2 } }, PerInstance };

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
		instanceVBO.Layout = &perInstanceLayout;
		instanceVBO.Size = size * size * sizeof(glm::vec2);
		instanceVBO.pTargetBuffer = quadBuffer;

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "TriangleIBO";
		ibo.Usage = BufferUsage::StaticDraw;
		ibo.Size = sizeof(indices);
		ibo.Data = indices;

		Ref<VertexBuffer> instanceBuffer = VertexBuffer::Create(&instanceVBO);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

		FrameBufferCreateInfo fboInfo = {};
		fboInfo.DebugName = "Framebuffer";
		fboInfo.AttachmentCount = 2;
		fboInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
		fboInfo.SwapChainTarget = true;

		Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(&fboInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "Main";
		renderPassInfo.pTargetFrameBuffer = frameBuffer;
		Ref<RenderPass> renderPass = RenderPass::Create(&renderPassInfo);

		PipelineSpecification spec = {};
		spec.DebugName = "InstancePipeline";
		spec.ShaderPath = "QuadInstanced.glsl";
		spec.DrawType = DrawType::Fill;
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.TargetRenderPass = renderPass;

		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		Ref<RenderCommandBuffer> commandBuffer = RenderCommandBuffer::CreateFromSwapchain();

		glm::vec2* transforms = new glm::vec2[size * size];

		float halfSize = (float)size / 2.0f;

		uint32_t index = 0;
		for (uint32_t x = 0; x < size; x++) {
			for (uint32_t y = 0; y < size; y++) {
				transforms[index] = { (float)x / (float)size - 0.5f, (float)y / (float)size - 0.5f };
				index++;
			}
		}

		instanceBuffer->SetData(transforms, size * size * sizeof(glm::vec2));

		while (running)
		{
			window->BeginFrame();
			window->GetContext()->BeginRenderPass(commandBuffer, renderPass);

			quadBuffer->Bind(commandBuffer);
			instanceBuffer->Bind(commandBuffer);
			indexBuffer->Bind(commandBuffer);
			pipeline->Bind(commandBuffer);
			pipeline->DrawInstanced(commandBuffer, indexBuffer->GetCount(), size * size);

			window->GetContext()->EndRenderPass(commandBuffer);
			window->Present();
		}

		std::cout << "Test closed";
	}
}

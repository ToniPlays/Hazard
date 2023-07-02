
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "TestUtils.h"
#include <filesystem>

#include <glad/glad.h>

using namespace HazardRenderer;

namespace TriangleTest {

	//OpenGL: Test
	//Vulkan: Working
	//Metal	: Test
	//DX12	: Test
	//DX11  : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;
        
		Window* window = CreateTestWindow("Triangle test", api, &running);
		window->Show();
		//---------------
        
		std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f
		};

		BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
								{ "a_Color", ShaderDataType::Float4 }
		};

        std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/triangle.glsl", api);

		BufferCreateInfo vbo = {};
		vbo.Name = "TriangleVBO";
		vbo.Size = sizeof(vertices);
		vbo.Data = vertices;
		vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

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

		{
			Ref<GPUBuffer> vertexBuffer = GPUBuffer::Create(&vbo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);

			while (running)
			{
				auto swapchain = window->GetSwapchain();
				auto commandBuffer = swapchain->GetSwapchainBuffer();
				auto renderPass = swapchain->GetRenderPass();
            
				Input::Update();
				window->BeginFrame();
            
				commandBuffer->BeginRenderPass(renderPass);
				commandBuffer->SetPipeline(pipeline);
				commandBuffer->SetVertexBuffer(vertexBuffer);
				commandBuffer->Draw(6);
				commandBuffer->EndRenderPass();
            
				Renderer::WaitAndRender();
				window->Present();
			}
		}
		window->Close();
	}
}

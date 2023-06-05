
#include "HazardRenderer.h"
#include "TestUtils.h"
#include "Event.h"
#include "Color.h"

#include "vendor/stb_image.h"

using namespace HazardRenderer;

namespace InstancingTest {

	//OpenGL : Working
	//Vulkan : Working
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	struct InstanceTransform
	{
		glm::vec4 MRow0;
		glm::vec4 MRow1;
		glm::vec4 MRow2;
	};


	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Instancing test", api, &running);
		window->Show();

		//---------------
		std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		BufferLayout layout = { { "a_Position",	ShaderDataType::Float3 },
								{ "a_Color",	ShaderDataType::Float4 },
								{ "a_MRow1",	ShaderDataType::Float4, PerInstance },
								{ "a_MRow2",	ShaderDataType::Float4, PerInstance },
								{ "a_MRow3",	ShaderDataType::Float4, PerInstance },
		};

		std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/instancing.glsl", api);

		std::vector<InstanceTransform> transforms(4);
		{
			glm::mat4 t = glm::translate(glm::mat4(1.0f), { -1.0f, 1.0f, -3.0f });

			transforms[0].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
			transforms[0].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
			transforms[0].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
		}
		{
			glm::mat4 t = glm::translate(glm::mat4(1.0f), { 1.0f, 1.0f, -3.0f });
			transforms[1].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
			transforms[1].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
			transforms[1].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
		}
		{
			glm::mat4 t = glm::translate(glm::mat4(1.0f), { 1.0f, -1.0f, -3.0f });
			transforms[2].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
			transforms[2].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
			transforms[2].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
		}
		{
			glm::mat4 t = glm::translate(glm::mat4(1.0f), { -1.0f, -1.0f, -3.0f });
			transforms[3].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
			transforms[3].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
			transforms[3].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
		}

		VertexBufferCreateInfo vbo = {};
		vbo.Name = "TriangleVBO";
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		VertexBufferCreateInfo instanceBufferSpec = {};
		instanceBufferSpec.Name = "InstanceVBO";
		instanceBufferSpec.Size = sizeof(InstanceTransform) * transforms.size();
		instanceBufferSpec.Data = transforms.data();

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

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Set = 0;
		uboInfo.Binding = 0;
		uboInfo.Size = sizeof(glm::mat4);

		{
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
			Ref<VertexBuffer> instancedBuffer = VertexBuffer::Create(&instanceBufferSpec);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);
			Ref<UniformBuffer> camera = UniformBuffer::Create(&uboInfo);

			while (running)
			{
				auto swapchain = window->GetSwapchain();
				auto commandBuffer = swapchain->GetSwapchainBuffer();
				auto renderPass = swapchain->GetRenderPass();

				float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
				glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.03f, 100.0f);

				BufferCopyRegion region = {};
				region.Data = &projection;
				region.Size = sizeof(glm::mat4);

				camera->SetData(region);

				Input::Update();
				window->BeginFrame();

				commandBuffer->BeginRenderPass(renderPass);
				commandBuffer->SetVertexBuffer(vertexBuffer);
				commandBuffer->SetVertexBuffer(instancedBuffer, 1);
				commandBuffer->SetPipeline(pipeline);
				commandBuffer->DrawInstanced(indexBuffer->GetCount(), 4, indexBuffer);
				commandBuffer->EndRenderPass();

				Renderer::WaitAndRender();
				window->Present();
			}
		}
		window->Close();
	}
}

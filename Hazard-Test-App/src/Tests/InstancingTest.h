
#include "HazardRenderer.h"
#include "TestUtils.h"
#include "Event.h"
#include "Color.h"

#include "vendor/stb_image.h"

using namespace HazardRenderer;

namespace InstancingTest {

	//OpenGL : Test
	//Vulkan : Test
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

		BufferCreateInfo vbo = {};
		vbo.Name = "TriangleVBO";
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;
		vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

		BufferCreateInfo instanceBufferSpec = {};
		instanceBufferSpec.Name = "InstanceVBO";
		instanceBufferSpec.Size = sizeof(InstanceTransform) * transforms.size();
		instanceBufferSpec.Data = transforms.data();
		instanceBufferSpec.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

		BufferCreateInfo ibo = {};
		ibo.Name = "TriangleIBO";
		ibo.Size = sizeof(indices);
		ibo.Data = indices;
		ibo.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT;

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

		BufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Size = sizeof(glm::mat4);
		uboInfo.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		DescriptorSetLayout descriptorLayout = {
			{ "u_Camera",  0, DESCRIPTOR_TYPE_UNIFORM_BUFFER	}
		};

		DescriptorSetCreateInfo descriptorSetSpec = {};
		descriptorSetSpec.Set = 0;
		descriptorSetSpec.pLayout = &descriptorLayout;

		{
			Ref<GPUBuffer> vertexBuffer = GPUBuffer::Create(&vbo);
			Ref<GPUBuffer> instancedBuffer = GPUBuffer::Create(&instanceBufferSpec);
			Ref<GPUBuffer> indexBuffer = GPUBuffer::Create(&ibo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);
			Ref<GPUBuffer> camera = GPUBuffer::Create(&uboInfo);
			Ref<DescriptorSet> descriptor = DescriptorSet::Create(&descriptorSetSpec);

			descriptor->Write(0, camera, true);

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
				commandBuffer->SetPipeline(pipeline);
				commandBuffer->SetDescriptorSet(descriptor, 0);
				commandBuffer->SetVertexBuffer(vertexBuffer);
				commandBuffer->SetVertexBuffer(instancedBuffer, 1);
				commandBuffer->DrawInstanced(indexBuffer->GetSize() / sizeof(uint32_t), 4, indexBuffer);
				commandBuffer->EndRenderPass();

				Renderer::WaitAndRender();
				window->Present();
			}
		}
		window->Close();
	}
}

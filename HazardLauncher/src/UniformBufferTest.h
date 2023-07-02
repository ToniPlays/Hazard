
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;

namespace UniformBufferTest
{

	//OpenGL : Test
	//Vulkan : Working
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Uniform buffer test", api, &running);
		window->Show();

		//---------------
		std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
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

		std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/UboTest.glsl", api);
		Hazard::TextureHeader header = Hazard::TextureFactory::LoadTextureFromSourceFile("assets/textures/csharp.png", true);

		BufferCreateInfo vbo = {};
		vbo.Name = "QuadVBO";
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;
		vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

		BufferCreateInfo ibo = {};
		ibo.Name = "QuadIBO";
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

		Image2DCreateInfo imageInfo = {};
		imageInfo.DebugName = "Image2D";
		imageInfo.Extent = { header.Width, header.Height, 1 };
		imageInfo.Format = ImageFormat::RGBA;
		imageInfo.Data = header.ImageData;
		imageInfo.Usage = ImageUsage::Texture;

		BufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Size = sizeof(glm::mat4);
		uboInfo.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		SamplerCreateInfo samplerInfo = {};
		samplerInfo.DebugName = "ImageSampler";
		samplerInfo.MinFilter = FilterMode::Linear;
		samplerInfo.MagFilter = FilterMode::Linear;
		samplerInfo.Wrapping = ImageWrap::ClampBorder;

		DescriptorSetLayout descriptorLayout = { 
			{ "u_Camera",  0, DESCRIPTOR_TYPE_UNIFORM_BUFFER	},
			{ "u_Texture", 1, DESCRIPTOR_TYPE_SAMPLER_2D		}
		};

		DescriptorSetCreateInfo descriptorSetSpec = {};
		descriptorSetSpec.Set = 0;
		descriptorSetSpec.pLayout = &descriptorLayout;

		{
			Ref<GPUBuffer> vertexBuffer = GPUBuffer::Create(&vbo);
			Ref<GPUBuffer> indexBuffer = GPUBuffer::Create(&ibo);
			Ref<GPUBuffer> camera = GPUBuffer::Create(&uboInfo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);
			Ref<Image2D> image = Image2D::Create(&imageInfo);
			Ref<Sampler> sampler = Sampler::Create(&samplerInfo);
			Ref<DescriptorSet> descriptorSet = DescriptorSet::Create(&descriptorSetSpec);

			descriptorSet->Write(0, camera, true);
			descriptorSet->Write(1, image, sampler, true);

			while (running)
			{
				auto swapchain = window->GetSwapchain();
				auto commandBuffer = swapchain->GetSwapchainBuffer();
				auto renderPass = swapchain->GetRenderPass();

				Input::Update();

				float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
				glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -10.0f, 10.0f);

				BufferCopyRegion region = {};
				region.Data = &projection;
				region.Size = sizeof(glm::mat4);

				camera->SetData(region);

				window->BeginFrame();
				commandBuffer->BeginRenderPass(renderPass);
				commandBuffer->SetPipeline(pipeline);
				commandBuffer->SetDescriptorSet(descriptorSet, 0);
				commandBuffer->SetVertexBuffer(vertexBuffer);
				commandBuffer->Draw(indexBuffer->GetSize() / sizeof(uint32_t), indexBuffer);
				commandBuffer->EndRenderPass();

				Renderer::WaitAndRender();
				window->Present();
			}
		}
		window->Close();
	}
}


#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;

namespace UniformBufferTest
{

	//OpenGL : Working
	//Vulkan : Working
	//Metal	 : Working
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

		stbi_set_flip_vertically_on_load(true);

		int w, h, channels;
		stbi_uc* data = stbi_load("assets/Textures/csharp.png", &w, &h, &channels, 4);

		VertexBufferCreateInfo vbo = {};
		vbo.Name = "QuadVBO";
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		IndexBufferCreateInfo ibo = {};
		ibo.Name = "QuadIBO";
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

		Image2DCreateInfo imageInfo = {};
		imageInfo.DebugName = "Image2D";
		imageInfo.Extent = { (uint32_t)w, (uint32_t)h, 1 };
		imageInfo.Format = ImageFormat::RGBA;
		imageInfo.Data = Buffer(data, w * h * 4);
		imageInfo.Usage = ImageUsage::Texture;

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Set = 0;
		uboInfo.Binding = 0;
		uboInfo.Size = sizeof(glm::mat4);

		{
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);
			Ref<Image2D> image = Image2D::Create(&imageInfo);
			Ref<UniformBuffer> camera = UniformBuffer::Create(&uboInfo);

			pipeline->GetShader()->Set("u_Texture", 0, image);

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
				commandBuffer->SetVertexBuffer(vertexBuffer);
				commandBuffer->SetPipeline(pipeline);
				commandBuffer->Draw(indexBuffer->GetCount(), indexBuffer);
				commandBuffer->EndRenderPass();

				Renderer::WaitAndRender();
				window->Present();
			}
		}

		window->Close();
	}
}

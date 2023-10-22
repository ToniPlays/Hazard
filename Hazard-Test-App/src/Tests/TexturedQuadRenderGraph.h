#pragma once

#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "TestUtils.h"

using namespace HazardRenderer;

namespace TexturedQuadRenderGraph
{

	//OpenGL : Test
	//Vulkan : Working
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Textured quad render graph", api, &running);
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

		std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/shaders/texturedQuad.glsl", api);
		Hazard::TextureHeader header = Hazard::TextureFactory::LoadTextureFromSourceFile("assets/textures/csharp.png", true);

		BufferCreateInfo vbo = {};
		vbo.Name = "TriangleVBO";
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;
		vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

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

		Image2DCreateInfo imageInfo = {};
		imageInfo.DebugName = "Image2D";
		imageInfo.Extent = { header.Width, header.Height, 1 };
		imageInfo.Format = ImageFormat::RGBA;
		imageInfo.Data = header.ImageData;
		imageInfo.Usage = ImageUsage::Texture;

		SamplerCreateInfo samplerInfo = {};
		samplerInfo.DebugName = "ImageSampler";
		samplerInfo.MinFilter = FilterMode::Linear;
		samplerInfo.MagFilter = FilterMode::Linear;
		samplerInfo.Wrapping = ImageWrap::ClampBorder;

		Ref<GPUBuffer> vertexBuffer = GPUBuffer::Create(&vbo);
		Ref<GPUBuffer> indexBuffer = GPUBuffer::Create(&ibo);
		Ref<Pipeline> pipeline = Pipeline::Create(&spec);
		Ref<Image2D> image = Image2D::Create(&imageInfo);
		Ref<Sampler> sampler = Sampler::Create(&samplerInfo);

		header.ImageData.Release();

		DescriptorSetLayout descriptorLayout = {
			{ "u_Texture", 0, DESCRIPTOR_TYPE_SAMPLER_2D }
		};


		DescriptorSetCreateInfo descriptorInfo = {};
		descriptorInfo.Set = 0;
		descriptorInfo.pLayout = &descriptorLayout;

		Ref<DescriptorSet> descriptorSet = DescriptorSet::Create(&descriptorInfo);
		descriptorSet->Write(0, 0, image, sampler, true);

		auto swapchain = window->GetSwapchain();
		auto frameBuffer = swapchain->GetRenderTarget();

		InputResource drawCalls = {};
		drawCalls.Name = "DrawCallData";
		drawCalls.UsageFlags = INPUT_RESOURCE_INSTRUCTIONS;

		RenderGraphStage meshStage = {};
		meshStage.DependencyCount = 0;
		meshStage.pDependencies = nullptr;
		meshStage.InputCount = 1;
		meshStage.pInputs = &drawCalls;

		RenderGraphCreateInfo graphInfo = {};
		graphInfo.DebugName = "RenderGraph";
		graphInfo.StageCount = 1;
		graphInfo.pStages = &meshStage;

		Ref<RenderGraph> graph = RenderGraph::Create(&graphInfo);

		while (running)
		{
			auto commandBuffer = swapchain->GetSwapchainBuffer();

			Input::Update();

			window->BeginFrame();
			commandBuffer->BeginRenderPass(swapchain->GetRenderPass());
			//graph->SetInput("DrawCallData", &list, sizeof(InstanceDrawCall));
			graph->Execute(commandBuffer);
			commandBuffer->EndRenderPass();

			Renderer::WaitAndRender();
			window->Present();
		}
		window->Close();
	}
}

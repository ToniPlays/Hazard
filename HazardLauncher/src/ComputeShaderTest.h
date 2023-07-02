
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include <filesystem>

#include <glad/glad.h>

using namespace HazardRenderer;

namespace ComputeShaderTest {

	//OpenGL: Test
	//Vulkan: Working
	//Metal : Test
	//DX12  : Test
	//DX11  : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Compute shader test", api, &running);
		window->Show();
		//---------------

		std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;

		BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
								{ "a_Color", ShaderDataType::Float4 },
								{ "a_TextureCoord", ShaderDataType::Float2 }
		};

		float vertices[] =
		{
			-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.75f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
		};

		BufferCreateInfo vbo = {};
		vbo.Name = "TriangleVBO";
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;
		vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

		std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/texturedQuad.glsl", api);

		PipelineSpecification spec = {};
		spec.DebugName = "Rasterized";
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.DrawType = DrawType::Fill;
		spec.CullMode = CullMode::None;
		spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
		spec.DepthTest = false;
		spec.pBufferLayout = &layout;
		spec.ShaderCodeCount = code.size();
		spec.pShaderCode = code.data();

		std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/compute.glsl", api);

		PipelineSpecification computeSpec = {};
		computeSpec.DebugName = "Compute";
		computeSpec.Usage = PipelineUsage::ComputeBit;
		computeSpec.ShaderCodeCount = computeCode.size();
		computeSpec.pShaderCode = computeCode.data();

		Image2DCreateInfo outputImageSpec = {};
		outputImageSpec.DebugName = "ComputeOutput";
		outputImageSpec.Extent = { 512, 512, 1 };
		outputImageSpec.Mips = 1;
		outputImageSpec.Format = ImageFormat::RGBA;
		outputImageSpec.Usage = ImageUsage::Storage;

		SamplerCreateInfo samplerInfo = {};
		samplerInfo.DebugName = "ImageSampler";
		samplerInfo.MinFilter = FilterMode::Linear;
		samplerInfo.MagFilter = FilterMode::Linear;
		samplerInfo.Wrapping = ImageWrap::ClampBorder;

		DescriptorSetLayout computeDescriptorLayout = {
			{ "o_OutputImage", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE }
		};

		DescriptorSetCreateInfo computeDescriptorSpec = {};
		computeDescriptorSpec.Set = 0;
		computeDescriptorSpec.pLayout = &computeDescriptorLayout;

		DescriptorSetLayout descriptorLayout = {
			{ "u_Texture", 0, DESCRIPTOR_TYPE_SAMPLER_2D }
		};

		DescriptorSetCreateInfo descriptorSpec = {};
		descriptorSpec.Set = 0;
		descriptorSpec.pLayout = &descriptorLayout;

		{
			Ref<GPUBuffer> vertexBuffer = GPUBuffer::Create(&vbo);
			Ref<Pipeline> pipeline = Pipeline::Create(&spec);
			Ref<Pipeline> compute = Pipeline::Create(&computeSpec);
			Ref<Image2D> outputImage = Image2D::Create(&outputImageSpec);
			Ref<Sampler> sampler = Sampler::Create(&samplerInfo);
			Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Compute", DeviceQueue::ComputeBit, 1);
			Ref<DescriptorSet> computeDescriptorSet = DescriptorSet::Create(&computeDescriptorSpec);
			Ref<DescriptorSet> descriptorSet = DescriptorSet::Create(&descriptorSpec);

			//Compute step
			{
				computeDescriptorSet->Write(0, outputImage, sampler);

				computeBuffer->Begin();
				computeBuffer->SetPipeline(compute);
				computeBuffer->SetDescriptorSet(computeDescriptorSet, 0);
				computeBuffer->DispatchCompute({ 512, 512, 1 });
				computeBuffer->End();
				computeBuffer->Submit();
			}

			descriptorSet->Write(0, outputImage, sampler, true);

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
				commandBuffer->SetDescriptorSet(descriptorSet, 0);
				commandBuffer->Draw(6);
				commandBuffer->EndRenderPass();

				Renderer::WaitAndRender();
				window->Present();
			}
		}
		window->Close();
	}
}


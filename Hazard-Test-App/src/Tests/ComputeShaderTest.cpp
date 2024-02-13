
#include "ComputeShaderTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "File.h"

#include <spdlog/fmt/fmt.h>

void ComputeShaderTest::Reset()
{

}
void ComputeShaderTest::Init()
{
	using namespace HazardRenderer;
	m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();

	m_Window->GetContext()->SetClearColor(Color(96, 96, 96, 255));
	m_Window->SetWindowTitle(GetName());

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

	std::unordered_map<RenderAPI, std::string> extensions = {
		{ RenderAPI::OpenGL, "ogl" },
		{ RenderAPI::Vulkan, "vk" },
		{ RenderAPI::Metal, "mtl" }
	};

	DescriptorSetLayout computeDescriptorLayout = {
		{ SHADER_STAGE_COMPUTE_BIT, "o_OutputImage", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE }
	};

	DescriptorSetLayout descriptorLayout = {
		{ SHADER_STAGE_FRAGMENT_BIT, "u_Texture", 0, DESCRIPTOR_TYPE_SAMPLER_2D }
	};


	BufferCreateInfo vbo = {};
	vbo.Name = "TriangleVBO";
	vbo.Size = sizeof(vertices);
	vbo.Data = &vertices;
	vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

	PipelineSpecification spec = {};
	spec.DebugName = "Rasterized";
	spec.Usage = PipelineUsage::GraphicsBit;
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
	spec.pBufferLayout = &layout;
	spec.SetLayouts = { descriptorLayout };
	spec.Flags = PIPELINE_DRAW_FILL | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	spec.Shaders = {
		{ SHADER_STAGE_VERTEX_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/texturedQuad.Vertex.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) },
		{ SHADER_STAGE_FRAGMENT_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/texturedQuad.Fragment.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) } };

	PipelineSpecification computeSpec = {};
	computeSpec.DebugName = "Compute";
	computeSpec.Usage = PipelineUsage::ComputeBit;
	computeSpec.SetLayouts = { computeDescriptorLayout };
	computeSpec.PushConstants = { { SHADER_STAGE_COMPUTE_BIT, 16, 0 } };
	computeSpec.Shaders = {
		{ SHADER_STAGE_COMPUTE_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/compute.Compute.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) } };


	Image2DCreateInfo outputImageSpec = {};
	outputImageSpec.DebugName = "ComputeOutput";
	outputImageSpec.Extent = { 512, 512, 1 };
	outputImageSpec.MaxMips = 1;
	outputImageSpec.Format = ImageFormat::RGBA;
	outputImageSpec.Usage = ImageUsage::Storage;

	SamplerCreateInfo samplerInfo = {};
	samplerInfo.DebugName = "ImageSampler";
	samplerInfo.MinFilter = FilterMode::Linear;
	samplerInfo.MagFilter = FilterMode::Linear;
	samplerInfo.Wrapping = ImageWrap::Repeat;

	DescriptorSetCreateInfo computeDescriptorSpec = {};
	computeDescriptorSpec.DebugName = "Compute";
	computeDescriptorSpec.Set = 0;
	computeDescriptorSpec.pLayout = &computeDescriptorLayout;

	DescriptorSetCreateInfo descriptorSpec = {};
	descriptorSpec.DebugName = "Graphics";
	descriptorSpec.Set = 0;
	descriptorSpec.pLayout = &descriptorLayout;

	m_VertexBuffer = GPUBuffer::Create(&vbo);
	m_Pipeline = Pipeline::Create(&spec);
	m_DescriptorSet = DescriptorSet::Create(&descriptorSpec);
	m_ComputeDescriptorSet = DescriptorSet::Create(&computeDescriptorSpec);
	m_ComputePipeline = Pipeline::Create(&computeSpec);
	m_OutputImage = Image2D::Create(&outputImageSpec);
	m_Sampler = Sampler::Create(&samplerInfo);

	Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Compute", DeviceQueue::ComputeBit);

	m_DescriptorSet->Write(0, 0, m_OutputImage, m_Sampler, true);
	m_ComputeDescriptorSet->Write(0, 0, m_OutputImage, nullptr);

	glm::vec4 color = Color("#1414ED");

	computeBuffer->Begin();
	computeBuffer->SetPipeline(m_ComputePipeline);
	computeBuffer->PushConstants(Buffer(&color, sizeof(glm::vec4)), 0, SHADER_STAGE_COMPUTE_BIT);
	computeBuffer->SetDescriptorSet(m_ComputeDescriptorSet, 0);
	computeBuffer->DispatchCompute({ m_OutputImage->GetWidth(), m_OutputImage->GetHeight(), 1 });
	computeBuffer->End();
	computeBuffer->Submit();

}
void ComputeShaderTest::Run()
{
	auto swapchain = m_Window->GetSwapchain();
	auto commandBuffer = swapchain->GetSwapchainBuffer();
	auto renderPass = swapchain->GetRenderPass();

	commandBuffer->BeginRenderPass(renderPass);
	commandBuffer->SetPipeline(m_Pipeline);
	commandBuffer->SetVertexBuffer(m_VertexBuffer);
	commandBuffer->SetDescriptorSet(m_DescriptorSet, 0);
	commandBuffer->Draw(6);
	commandBuffer->EndRenderPass();
}
void ComputeShaderTest::Terminate()
{

}

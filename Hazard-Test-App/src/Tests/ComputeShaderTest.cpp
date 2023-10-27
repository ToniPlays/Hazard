
#include "ComputeShaderTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"

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


	BufferCreateInfo vbo = {};
	vbo.Name = "TriangleVBO";
	vbo.Size = sizeof(vertices);
	vbo.Data = &vertices;
	vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

	//std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/shaders/texturedQuad.glsl", m_Window->GetWindowInfo().SelectedAPI);

	PipelineSpecification spec = {};
	spec.DebugName = "Rasterized";
	spec.Usage = PipelineUsage::GraphicsBit;
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
	spec.pBufferLayout = &layout;
	//spec.ShaderCodeCount = code.size();
	//spec.pShaderCode = code.data();

	//std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/shaders/compute.glsl", m_Window->GetWindowInfo().SelectedAPI);

	PipelineSpecification computeSpec = {};
	computeSpec.DebugName = "Compute";
	computeSpec.Usage = PipelineUsage::ComputeBit;
	//computeSpec.ShaderCodeCount = computeCode.size();
	//computeSpec.pShaderCode = computeCode.data();

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
	computeDescriptorSpec.DebugName = "Compute";
	computeDescriptorSpec.Set = 0;
	computeDescriptorSpec.pLayout = &computeDescriptorLayout;

	DescriptorSetLayout descriptorLayout = {
		{ "u_Texture", 0, DESCRIPTOR_TYPE_SAMPLER_2D }
	};

	DescriptorSetCreateInfo descriptorSpec = {};
	descriptorSpec.DebugName = "Graphics";
	descriptorSpec.Set = 0;
	descriptorSpec.pLayout = &descriptorLayout;

	m_VertexBuffer = GPUBuffer::Create(&vbo);
	//m_Pipeline = Pipeline::Create(&spec);
	m_DescriptorSet = DescriptorSet::Create(&descriptorSpec);
	m_ComputeDescriptorSet = DescriptorSet::Create(&computeDescriptorSpec);
	//m_ComputePipeline = Pipeline::Create(&computeSpec);
	m_OutputImage = Image2D::Create(&outputImageSpec);
	m_Sampler = Sampler::Create(&samplerInfo);

	Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Compute", DeviceQueue::ComputeBit);

	m_DescriptorSet->Write(0, 0, m_OutputImage, m_Sampler, true);
	m_ComputeDescriptorSet->Write(0, 0, m_OutputImage, nullptr);

	computeBuffer->Begin();
	//computeBuffer->SetPipeline(m_ComputePipeline);
	//computeBuffer->SetDescriptorSet(m_ComputeDescriptorSet, 0);
	//computeBuffer->DispatchCompute({ m_OutputImage->GetWidth(), m_OutputImage->GetHeight(), 1 });
	computeBuffer->End();
	computeBuffer->Submit();

}
void ComputeShaderTest::Run()
{
	auto swapchain = m_Window->GetSwapchain();
	auto commandBuffer = swapchain->GetSwapchainBuffer();
	auto renderPass = swapchain->GetRenderPass();

	commandBuffer->BeginRenderPass(renderPass);
	//commandBuffer->SetPipeline(m_Pipeline);
	//commandBuffer->SetVertexBuffer(m_VertexBuffer);
	//commandBuffer->SetDescriptorSet(m_DescriptorSet, 0);
	//commandBuffer->Draw(6);
	commandBuffer->EndRenderPass();
}
void ComputeShaderTest::Terminate()
{

}

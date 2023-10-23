#include "UniformBufferTest.h"

#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/RenderContext/TextureFactory.h"

void UniformBufferTest::Reset()
{

}

void UniformBufferTest::Init()
{
	using namespace HazardRenderer;
	m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();

	m_Window->GetContext()->SetClearColor(Color(255, 128, 0, 255));
	m_Window->SetWindowTitle(GetName());

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

	std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/shaders/UboTest.glsl", m_Window->GetWindowInfo().SelectedAPI);
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
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
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
	descriptorSetSpec.DebugName = "Camera";
	descriptorSetSpec.Set = 0;
	descriptorSetSpec.pLayout = &descriptorLayout;

	m_VertexBuffer = GPUBuffer::Create(&vbo);
	m_IndexBuffer = GPUBuffer::Create(&ibo);
	m_UniformBuffer = GPUBuffer::Create(&uboInfo);
	m_Pipeline = Pipeline::Create(&spec);
	m_Image = Image2D::Create(&imageInfo);
	m_Sampler = Sampler::Create(&samplerInfo);
	m_DescriptorSet = DescriptorSet::Create(&descriptorSetSpec);

	m_DescriptorSet->Write(0, m_UniformBuffer, true);
	m_DescriptorSet->Write(1, 0, m_Image, m_Sampler, true);
}

void UniformBufferTest::Run()
{
	auto swapchain = m_Window->GetSwapchain();
	auto commandBuffer = swapchain->GetSwapchainBuffer();
	auto renderPass = swapchain->GetRenderPass();

	float aspectRatio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
	glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -10.0f, 10.0f);

	BufferCopyRegion region = {};
	region.Data = &projection;
	region.Size = sizeof(glm::mat4);

	m_UniformBuffer->SetData(region);

	commandBuffer->BeginRenderPass(renderPass);
	commandBuffer->SetPipeline(m_Pipeline);
	commandBuffer->SetDescriptorSet(m_DescriptorSet, 0);
	commandBuffer->SetVertexBuffer(m_VertexBuffer);
	commandBuffer->Draw(m_IndexBuffer->GetSize() / sizeof(uint32_t), m_IndexBuffer);
	commandBuffer->EndRenderPass();
}

void UniformBufferTest::Terminate()
{

}

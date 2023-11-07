#include "UniformBufferTest.h"

#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include <spdlog/fmt/fmt.h>


void UniformBufferTest::Reset()
{

}

void UniformBufferTest::Init()
{
	using namespace HazardRenderer;
	m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();

	m_Window->GetContext()->SetClearColor(Color("#14ED14"));
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

	std::unordered_map<RenderAPI, std::string> extensions = {
		{ RenderAPI::OpenGL, "ogl" },
		{ RenderAPI::Vulkan, "vk" },
		{ RenderAPI::Metal, "mtl" }
	};


	DescriptorSetLayout descriptorLayout = {
		{ SHADER_STAGE_VERTEX_BIT, "u_Camera",  0, DESCRIPTOR_TYPE_UNIFORM_BUFFER	},
		{ SHADER_STAGE_FRAGMENT_BIT, "u_Texture", 1, DESCRIPTOR_TYPE_SAMPLER_2D		}
	};

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
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
	spec.pBufferLayout = &layout;
	spec.SetLayouts = { descriptorLayout };
	spec.Flags = PIPELINE_DRAW_FILL | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	spec.Shaders = {
		{ SHADER_STAGE_VERTEX_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/UboTest.Vertex.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) },
		{ SHADER_STAGE_FRAGMENT_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/UboTest.Fragment.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) } };


	Image2DCreateInfo imageInfo = {};
	imageInfo.DebugName = "Image2D";
    imageInfo.Extent = header.Extent;
	imageInfo.Format = ImageFormat::RGBA;
	imageInfo.Data = header.ImageData;
	imageInfo.Usage = ImageUsage::Texture;

	BufferCreateInfo uboInfo = {};
	uboInfo.Name = "Camera";
	uboInfo.Size = sizeof(glm::mat4);
	uboInfo.UsageFlags = BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_DYNAMIC;

	SamplerCreateInfo samplerInfo = {};
	samplerInfo.DebugName = "ImageSampler";
	samplerInfo.MinFilter = FilterMode::Linear;
	samplerInfo.MagFilter = FilterMode::Linear;
	samplerInfo.Wrapping = ImageWrap::ClampBorder;

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
    
    header.ImageData.Release();
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

#include "TexturedQuad.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include <spdlog/fmt/fmt.h>

void TexturedQuadTest::Reset()
{

}

void TexturedQuadTest::Init()
{
	using namespace HazardRenderer;

	m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();

	m_Window->GetContext()->SetClearColor(Color("#ED1414"));
	m_Window->SetWindowTitle(GetName());

	std::cout << "Selected device: " << m_Window->GetContext()->GetDevice()->GetDeviceName() << std::endl;

	std::unordered_map<RenderAPI, std::string> extensions = {
		{ RenderAPI::OpenGL, "ogl" },
		{ RenderAPI::Vulkan, "vk" },
		{ RenderAPI::Metal, "mtl" }
	};

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

	DescriptorSetLayout descriptorLayout = {
		{ SHADER_STAGE_FRAGMENT_BIT, "u_Texture", 0, DESCRIPTOR_TYPE_SAMPLER_2D, }
	};

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
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
	spec.pBufferLayout = &layout;
	spec.SetLayouts = { descriptorLayout };
	spec.Flags = PIPELINE_DRAW_FILL | PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	spec.Shaders = {
		{ SHADER_STAGE_VERTEX_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/texturedQuad.Vertex.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) },
		{ SHADER_STAGE_FRAGMENT_BIT, File::ReadFile(fmt::format("assets/compiled/shaders/texturedQuad.Fragment.{}", extensions[m_Window->GetWindowInfo().SelectedAPI])) }};

	Image2DCreateInfo imageInfo = {};
	imageInfo.DebugName = "Image2D";
    imageInfo.Extent = header.Extent;
	imageInfo.Format = ImageFormat::RGBA;
	imageInfo.Data = header.ImageData;
	imageInfo.Usage = ImageUsage::Texture;

	SamplerCreateInfo samplerInfo = {};
	samplerInfo.DebugName = "ImageSampler";
	samplerInfo.MinFilter = FilterMode::Linear;
	samplerInfo.MagFilter = FilterMode::Linear;
	samplerInfo.Wrapping = ImageWrap::Repeat;

	m_VertexBuffer = GPUBuffer::Create(&vbo);
	m_IndexBuffer = GPUBuffer::Create(&ibo);
	m_Pipeline = Pipeline::Create(&spec);
	m_Image = Image2D::Create(&imageInfo);
	m_Sampler = Sampler::Create(&samplerInfo);

	header.ImageData.Release();

	DescriptorSetCreateInfo descriptorInfo = {};
	descriptorInfo.DebugName = "Descriptor";
	descriptorInfo.Set = 0;
	descriptorInfo.pLayout = &descriptorLayout;

	m_DescriptorSet = DescriptorSet::Create(&descriptorInfo);
	m_DescriptorSet->Write(0, 0, m_Image, m_Sampler, true);
    
    header.ImageData.Release();
}

void TexturedQuadTest::Run()
{
	auto swapchain = m_Window->GetSwapchain();
	auto commandBuffer = swapchain->GetSwapchainBuffer();

	commandBuffer->BeginRenderPass(swapchain->GetRenderPass());
	commandBuffer->SetPipeline(m_Pipeline);
	commandBuffer->SetDescriptorSet(m_DescriptorSet, 0);
	commandBuffer->SetVertexBuffer(m_VertexBuffer);
	commandBuffer->Draw(m_IndexBuffer->GetSize() / sizeof(uint32_t), m_IndexBuffer);
	commandBuffer->EndRenderPass();
}

void TexturedQuadTest::Terminate()
{

}

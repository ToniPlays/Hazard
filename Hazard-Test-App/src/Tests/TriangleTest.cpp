#include "TriangleTest.h"
#include "Hazard/Core/Application.h"
#include "Hazard/RenderContext/RenderContextManager.h"

void TriangleTest::Reset()
{

}

void TriangleTest::Init()
{
	using namespace HazardRenderer;
	m_Window = &Hazard::Application::Get().GetModule<Hazard::RenderContextManager>().GetWindow();

	m_Window->GetContext()->SetClearColor(Color(255, 128, 0, 255));
	m_Window->SetWindowTitle(GetName());

	std::cout << "Selected device: " << m_Window->GetContext()->GetDevice()->GetDeviceName() << std::endl;

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f
	};

	BufferLayout layout = { { "a_Position", ShaderDataType::Float3 },
							{ "a_Color",	ShaderDataType::Float4 }
	};

	//std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/shaders/triangle.glsl", m_Window->GetWindowInfo().SelectedAPI);

	BufferCreateInfo vbo = {};
	vbo.Name = "TriangleVBO";
	vbo.Size = sizeof(vertices);
	vbo.Data = vertices;
	vbo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

	PipelineSpecification spec = {};
	spec.DebugName = "Pipeline";
	spec.Usage = PipelineUsage::GraphicsBit;
	spec.DrawType = DrawType::Fill;
	spec.CullMode = CullMode::None;
	spec.pTargetRenderPass = m_Window->GetSwapchain()->GetRenderPass().Raw();
	spec.DepthTest = false;
	spec.pBufferLayout = &layout;
	//spec.ShaderCodeCount = code.size();
	//spec.pShaderCode = code.data();

	{
		m_VertexBuffer = GPUBuffer::Create(&vbo);
		//m_Pipeline = Pipeline::Create(&spec);
	}
}

void TriangleTest::Run()
{
	auto swapchain = m_Window->GetSwapchain();
	auto commandBuffer = swapchain->GetSwapchainBuffer();

	commandBuffer->BeginRenderPass(swapchain->GetRenderPass());
	//commandBuffer->SetPipeline(m_Pipeline);
	//commandBuffer->SetVertexBuffer(m_VertexBuffer);
	//commandBuffer->Draw(6);
	commandBuffer->EndRenderPass();
}

void TriangleTest::Terminate()
{
    
}

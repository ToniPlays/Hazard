#include "OpenGLComputePipeline.h"
#include "Backend/Core/Renderer.h"

namespace HazardRenderer::OpenGL
{
	OpenGLComputePipeline::OpenGLComputePipeline(ComputePipelineCreateInfo* createInfo)
	{
		m_Shader = Shader::Create(createInfo->ShaderPath);
	}
	OpenGLComputePipeline::~OpenGLComputePipeline()
	{

	}
	void OpenGLComputePipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
	{
		m_Shader->Bind(commandBuffer);
	}
	void OpenGLComputePipeline::Execute(Ref<RenderCommandBuffer> commandBuffer)
	{
		Ref<OpenGLComputePipeline> instance = this;
		Renderer::Submit([instance]() mutable {
			Timer timer;
			glDispatchCompute(32, 32, 6);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			std::cout << "Compute took: " << timer.ElapsedMillis() << "ms" << std::endl;
			});
	}
}
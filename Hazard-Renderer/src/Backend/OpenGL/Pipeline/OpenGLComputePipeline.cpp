#include "OpenGLComputePipeline.h"
#include "Backend/Core/Renderer.h"

namespace HazardRenderer::OpenGL
{
	OpenGLComputePipeline::OpenGLComputePipeline(ComputePipelineCreateInfo* createInfo) : m_Spec(*createInfo)
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
	void OpenGLComputePipeline::Execute(Ref<RenderCommandBuffer> commandBuffer, const glm::vec3& size)
	{
		Ref<OpenGLComputePipeline> instance = this;
		Renderer::Submit([instance, s = size]() mutable {
			Timer timer;
			glDispatchCompute(s.x, s.y, s.z);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			std::cout << "Compute took: " << timer.ElapsedMillis() << "ms (" << instance->GetSpec().DebugName << ")" << std::endl;
			});
	}
}
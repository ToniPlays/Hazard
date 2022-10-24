
#include "OpenGLRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_OPENGL
#include "OpenGLSwapchain.h"
#include "OpenGLCore.h"
#include "OpenGLUtils.h"

#include "Backend/Core/Renderer.h"

namespace HazardRenderer::OpenGL
{
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(uint32_t size, const std::string& name)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(const std::string& name, bool swapchain)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	void OpenGLRenderCommandBuffer::Begin()
	{
		m_FrameIndex = m_Swapchain.As<OpenGLSwapchain>()->GetFrameIndex();
	}
	void OpenGLRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		Ref<OpenGLRenderPass> instance = renderPass.As<OpenGLRenderPass>();
		Renderer::Submit([instance, explicitClear]() mutable {
			instance->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
			});
	}
	void OpenGLRenderCommandBuffer::BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear)
	{
		renderPass->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
	}
	void OpenGLRenderCommandBuffer::EndRenderPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLRenderCommandBuffer::BindVertexBuffer(Ref<VertexBuffer> vertexBuffer, uint32_t binding)
	{
		Ref<OpenGLVertexBuffer> instance = vertexBuffer.As<OpenGLVertexBuffer>();
		Renderer::Submit([instance, binding]() {
			uint32_t vao = instance->GetVAOID();
			if (vao && vao != s_BoundVAO)
			{
				glBindVertexArray(vao);
				s_BoundVAO = vao;
				s_CurrentLayout = instance->GetLayout();
			}
			glVertexArrayVertexBuffer(s_BoundVAO, binding, instance->GetBufferID(), 0, s_CurrentLayout.GetBufferStride(binding));
			});
	}
	void OpenGLRenderCommandBuffer::BindUniformBuffer(Ref<UniformBuffer> uniformBuffer)
	{
		Ref<OpenGLUniformBuffer> instance = uniformBuffer.As<OpenGLUniformBuffer>();
		Renderer::Submit([instance]() {
			glBindBufferBase(GL_UNIFORM_BUFFER, instance->GetBinding(), instance->GetBufferID());
			});
	}
	void OpenGLRenderCommandBuffer::BindPipeline(Ref<Pipeline> pipeline)
	{
		Ref<OpenGLPipeline> instance = pipeline.As<OpenGLPipeline>();
		Renderer::Submit([instance]() mutable {

			Ref<OpenGLShader> shader = instance->GetShader().As<OpenGLShader>();
			glUseProgram(shader->GetProgramID());
			auto& spec = instance->GetSpecifications();

			if (spec.DepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(OpenGLUtils::GetGLDepthFunc(spec.DepthOperator));
			}
			else glDisable(GL_DEPTH_TEST);

			glDepthMask(instance->DepthMaskEnable());
			for (auto& [index, descriptor] : shader->GetDescriptorSets())
				descriptor.BindResources(shader->GetProgramID(), spec.Usage == PipelineUsage::ComputeBit);
			});
		m_CurrentPipeline = instance;
	}
	void OpenGLRenderCommandBuffer::Draw(uint32_t count, Ref<IndexBuffer> indexBuffer)
	{
		Ref<OpenGLIndexBuffer> instance = indexBuffer.As<OpenGLIndexBuffer>();
		Ref<OpenGLPipeline> pipeline = m_CurrentPipeline;

		Renderer::Submit([instance, count, pipeline]() mutable {
			if (instance)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->GetBufferID());
				glDrawElements(pipeline->GetDrawType(), count, GL_UNSIGNED_INT, nullptr);
			}
			else glDrawArrays(pipeline->GetDrawType(), 0, count);
			});
	}
	void OpenGLRenderCommandBuffer::DrawInstanced(uint32_t count, uint32_t instanceCount, Ref<IndexBuffer> indexBuffer)
	{
		Ref<OpenGLIndexBuffer> indexBufferInstance = indexBuffer.As<OpenGLIndexBuffer>();
		Ref<OpenGLPipeline> pipeline = m_CurrentPipeline;

		Renderer::Submit([indexBufferInstance, count, instanceCount, pipeline]() mutable {
			if (indexBufferInstance)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferInstance->GetBufferID());
				glDrawElementsInstanced(pipeline->GetDrawType(), count, GL_UNSIGNED_INT, nullptr, instanceCount);
			}
			else glDrawArraysInstanced(pipeline->GetDrawType(), 0, count, instanceCount);
			});
	}
	void OpenGLRenderCommandBuffer::DispatchCompute(const LocalGroupSize& localGroupSize)
	{
		HZR_ASSERT(m_CurrentPipeline->GetSpecifications().Usage == PipelineUsage::ComputeBit, "Pipeline is not a compute");
		Ref<OpenGLPipeline> pipeline = m_CurrentPipeline;
		Renderer::Submit([pipeline, size = localGroupSize]() mutable {

			glDispatchCompute(size.x, size.y, size.z);
			});
	}
	void OpenGLRenderCommandBuffer::InsertMemoryBarrier(MemoryBarrierFlags flags)
	{
		Renderer::Submit([flags]() mutable {
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			});
	}
}
#endif
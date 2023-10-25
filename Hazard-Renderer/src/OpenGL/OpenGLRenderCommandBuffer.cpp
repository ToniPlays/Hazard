
#include "OpenGLRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_OPENGL
#include "OpenGLSwapchain.h"
#include "OpenGLCore.h"
#include "OpenGLUtils.h"

#include "Backend/Core/Renderer.h"

namespace HazardRenderer::OpenGL
{
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(const std::string& name, DeviceQueue queue, uint32_t size)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(const std::string& name, bool swapchain)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	void OpenGLRenderCommandBuffer::Begin()
	{
		HZR_PROFILE_FUNCTION();
		m_FrameIndex = m_Swapchain.As<OpenGLSwapchain>()->GetFrameIndex();
	}
	void OpenGLRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLRenderPass> instance = renderPass.As<OpenGLRenderPass>();
		Renderer::Submit([instance, explicitClear]() mutable {
			instance->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
			});
	}
	void OpenGLRenderCommandBuffer::BeginRenderPass_RT(Ref<RenderPass> renderPass, bool explicitClear)
	{
		HZR_PROFILE_FUNCTION();
		renderPass->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
	}
	void OpenGLRenderCommandBuffer::EndRenderPass()
	{
		HZR_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLRenderCommandBuffer::SetVertexBuffer(Ref<GPUBuffer> vertexBuffer, uint32_t binding)
	{
		HZR_PROFILE_FUNCTION();
		/*
		Ref<OpenGLVertexBuffer> instance = vertexBuffer.As<OpenGLVertexBuffer>();
		Renderer::Submit([instance, binding]() {
			uint32_t vao = instance->GetVAOID();
			if (binding == 0)
			{
				glBindVertexArray(vao);
				s_BoundVAO = vao;
				s_CurrentLayout = instance->GetLayout();
			}
			glVertexArrayVertexBuffer(s_BoundVAO, binding, instance->GetBufferID(), 0, s_CurrentLayout.GetBufferStride(binding));
			});
			*/
	}
	/*
	void OpenGLRenderCommandBuffer::SetUniformBuffers(const Ref<GPUBuffer>* uniformBuffer, uint32_t count)
	{
		HZR_PROFILE_FUNCTION();
		std::vector<Ref<UniformBuffer>> buffers(count);
		memcpy(buffers.data(), uniformBuffer, count * sizeof(Ref<GPUBuffer>));
		Renderer::Submit([buffers]() {
			for(auto& buffer : buffers)
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer->GetBinding(), buffer.As<OpenGLUniformBuffer>()->GetBufferID());
			});
	}
	*/
	void OpenGLRenderCommandBuffer::SetPipeline(Ref<Pipeline> pipeline)
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLRenderCommandBuffer> instance = this;
		m_CurrentPipeline = pipeline.As<OpenGLPipeline>();

		Renderer::Submit([instance]() mutable {
			Ref<OpenGLShader> shader = instance->m_CurrentPipeline->GetShader().As<OpenGLShader>();
			glUseProgram(shader->GetProgramID());
			auto spec = instance->m_CurrentPipeline->GetSpecifications();

			if (spec.CullMode == CullMode::None)
			{
				glDisable(GL_CULL_FACE);
			}
			else
			{
				glEnable(GL_CULL_FACE);
				glCullFace(spec.CullMode == CullMode::FrontFace ? GL_FRONT : GL_BACK);
			}
			if (spec.DepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(OpenGLUtils::GetGLDepthFunc(spec.DepthOperator));
			}
			else glDisable(GL_DEPTH_TEST);

			glDepthMask(instance->m_CurrentPipeline->DepthMaskEnable());
			for (auto& [index, descriptor] : shader->GetDescriptorSets())
				descriptor.BindResources(shader->GetProgramID(), false);
			});
	}
	void OpenGLRenderCommandBuffer::Draw(uint64_t count, Ref<GPUBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		/*
		Ref<OpenGLIndexBuffer> instance = indexBuffer.As<OpenGLIndexBuffer>();
		Ref<OpenGLPipeline> pipeline = m_CurrentPipeline;

		Renderer::Submit([instance, count, pipeline]() mutable {
			if (instance)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->GetBufferID());
				glDrawElements(pipeline->GetDrawType(), (GLsizei)count, GL_UNSIGNED_INT, nullptr);
			}
			else glDrawArrays(pipeline->GetDrawType(), 0, (GLsizei)count);
			});
			*/
	}
	void OpenGLRenderCommandBuffer::DrawInstanced(uint64_t count, uint32_t instanceCount, Ref<GPUBuffer> indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
		/*
		Ref<OpenGLIndexBuffer> indexBufferInstance = indexBuffer.As<OpenGLIndexBuffer>();
		Ref<OpenGLPipeline> pipeline = m_CurrentPipeline;

		Renderer::Submit([indexBufferInstance, count, instanceCount, pipeline]() mutable {
			if (indexBufferInstance)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferInstance->GetBufferID());
				glDrawElementsInstanced(pipeline->GetDrawType(), (GLsizei)count, GL_UNSIGNED_INT, nullptr, instanceCount);
			}
			else glDrawArraysInstanced(pipeline->GetDrawType(), 0, (GLsizei)count, instanceCount);
			});
			*/
	}
	void OpenGLRenderCommandBuffer::DispatchCompute(GroupSize globalGroupSize)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(m_CurrentPipeline->GetSpecifications().Usage == PipelineUsage::ComputeBit, "Pipeline is not a compute");
		
		Ref<OpenGLRenderCommandBuffer> instance = this;

		Renderer::Submit([instance, size = globalGroupSize]() mutable {

			auto shader = instance->m_CurrentPipeline->GetShader().As<OpenGLShader>();
			glUseProgram(shader->GetProgramID());

			for (auto& [index, descriptor] : shader->GetDescriptorSets())
				descriptor.BindResources(shader->GetProgramID(), true);

			glDispatchCompute(size.x, size.y, size.z);
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			});
	}
	/*void OpenGLRenderCommandBuffer::InsertMemoryBarrier(const MemoryBarrierInfo& info)
	{
		HZR_PROFILE_FUNCTION();
		Renderer::Submit([info]() mutable {
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
			});
	}*/
}
#endif

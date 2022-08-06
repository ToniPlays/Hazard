
#include "OpenGLBuffers.h"

#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Renderer.h"
#include "Backend/Core/Pipeline/ShaderDataType.h"
#include "../OpenGLContext.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		HZR_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(VertexBufferCreateInfo* info) : m_Size(info->Size)
	{

		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		if (info->Layout != nullptr)
			m_Layout = *info->Layout;

		uint32_t offset = 0;
		Ref<OpenGLVertexBuffer> instance = this;

		Renderer::SubmitResourceCreate([instance, data = info->Data]() mutable {

			if (instance->m_Layout.GetStride() == 0)
			{
				glCreateBuffers(1, &instance->m_ID);
				glNamedBufferData(instance->m_ID, instance->m_Size * 4530043, nullptr, GL_STREAM_DRAW + instance->m_Usage);
				return;
			}

			glCreateVertexArrays(1, &instance->m_VAO);
			glCreateBuffers(1, &instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, nullptr, GL_STREAM_DRAW + instance->m_Usage);


			uint32_t bufferedOffset = 0;
			uint32_t currentBuffer = 0;

			for (uint32_t i = 0; i < instance->m_Layout.GetElementCount(); i++)
			{
				BufferElement element = instance->m_Layout.GetElements()[i];

				uint32_t stride = instance->m_Layout.GetBufferStride(element.ElementDivisor);

				if (element.ElementDivisor != currentBuffer) {
					bufferedOffset += element.Offset;
					currentBuffer = element.ElementDivisor;
					glVertexArrayBindingDivisor(instance->m_VAO, element.ElementDivisor, element.ElementDivisor);
				}

				glEnableVertexArrayAttrib(instance->m_VAO, i);
				glVertexArrayAttribFormat(instance->m_VAO, i, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, element.Offset - bufferedOffset);
				glVertexArrayAttribBinding(instance->m_VAO, i, element.ElementDivisor);
			}

			if (data) {
				instance->SetData_RT(data, instance->m_Size);
			}
		});
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		Renderer::SubmitResourceFree([id = m_ID, vao = m_VAO]() mutable {
			glDeleteBuffers(1, &id);
			glDeleteVertexArrays(1, &vao);
			});
	}
	void OpenGLVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer, uint32_t binding)
	{
		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance, binding]() {
			if (instance->m_VAO)
			{
				if (instance->m_VAO != s_BoundVAO) {
					glBindVertexArray(instance->m_VAO);
					s_BoundVAO = instance->m_VAO;
					s_CurrentLayout = instance->m_Layout;
				}
			}
			glVertexArrayVertexBuffer(s_BoundVAO, binding, instance->m_ID, 0, s_CurrentLayout.GetBufferStride(binding));
			});
	}
	void OpenGLVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		Renderer::Submit([]() mutable {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			});
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, data, size]() mutable {
			glNamedBufferSubData(instance->m_ID, 0, size, data);
			});
	}
	void OpenGLVertexBuffer::SetData_RT(const void* data, uint32_t size)
	{
		glNamedBufferSubData(m_ID, 0, size, data);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		Ref<OpenGLIndexBuffer> instance = this;

		Renderer::SubmitResourceCreate([instance, data = info->Data]() mutable {

			glCreateBuffers(1, &instance->m_ID);

			if (data != nullptr)
				instance->SetData_RT(data, instance->m_Size);

		});
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			glDeleteBuffers(1, &instance->m_ID);
			});
	}
	void OpenGLIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_ID);
			});
	}
	void OpenGLIndexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		Renderer::Submit([]() mutable {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			});
	}
	void OpenGLIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, data, size]() mutable {
			instance->m_Size = size;
			glNamedBufferData(instance->m_ID, size, data, GL_STREAM_DRAW + instance->m_Usage);
			});
	}
	void OpenGLIndexBuffer::SetData_RT(uint32_t* data, uint32_t size)
	{
		m_Size = size;
		glNamedBufferData(m_ID, size, data, GL_STREAM_DRAW + m_Usage);
	}
	OpenGLUniformBuffer::OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Name(createInfo->Name), m_Size(createInfo->Size),
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{
		m_LocalData.Allocate(m_Size * 16);
		m_LocalData.ZeroInitialize();

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			glCreateBuffers(1, &instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_LocalData.Size, nullptr, GL_DYNAMIC_DRAW);
			});
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		m_LocalData.Release();
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			glDeleteBuffers(1, &instance->m_ID);
			});
	}
	void OpenGLUniformBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		if (m_FrameIndex != cmdBuffer->GetFrameIndex()) {
			m_CurrentBufferDataIndex = 0;
			m_FrameIndex = cmdBuffer->GetFrameIndex();
		}

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			glBindBufferBase(GL_UNIFORM_BUFFER, instance->m_Binding, instance->m_ID);
			});
	}
	void OpenGLUniformBuffer::Bind_RT(Ref<RenderCommandBuffer> cmdBuffer)
	{
		if (m_FrameIndex != cmdBuffer->GetFrameIndex()) {
			m_CurrentBufferDataIndex = 0;
			m_FrameIndex = cmdBuffer->GetFrameIndex();
		}
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_ID);
	}
	void OpenGLUniformBuffer::Unbind()
	{

	}
	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size)
	{
		HZR_PROFILE_FUNCTION();
		m_LocalData.Write(data, size, m_CurrentBufferDataIndex);

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, startIndex = m_CurrentBufferDataIndex]() mutable {

			uint32_t size = instance->m_Size;
			HZR_PROFILE_FUNCTION("OpenGLUniformBuffer::SetData(const void*, uint32_t)_RT");
			glNamedBufferData(instance->m_ID, size, (byte*)instance->m_LocalData.Data + startIndex, GL_DYNAMIC_DRAW);
			});
		m_CurrentBufferDataIndex += m_Size;
	}
}
#endif

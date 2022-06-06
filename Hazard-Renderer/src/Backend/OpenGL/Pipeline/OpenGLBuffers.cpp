
#include "OpenGLBuffers.h"

#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Pipeline/ShaderDataType.h"
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

		if (m_Layout.GetStride() == 0)
		{
			glCreateBuffers(1, &m_ID);
			glNamedBufferData(m_ID, m_Size, nullptr, GL_STREAM_DRAW + m_Usage);
			return;
		}


		glCreateVertexArrays(1, &m_VAO);
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, m_Size, nullptr, GL_STREAM_DRAW + m_Usage);

		uint32_t bufferedOffset = 0;
		uint32_t currentBuffer = 0;

		for (uint32_t i = 0; i < m_Layout.GetElementCount(); i++)
		{
			BufferElement element = m_Layout.GetElements()[i];

			uint32_t stride = m_Layout.GetBufferStride(element.ElementDivisor);

			if (element.ElementDivisor != currentBuffer) {
				bufferedOffset += element.Offset;
				currentBuffer = element.ElementDivisor;
				glVertexArrayBindingDivisor(m_VAO, element.ElementDivisor, element.ElementDivisor);
			}

			glEnableVertexArrayAttrib(m_VAO, i);
			glVertexArrayAttribFormat(m_VAO, i, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, element.Offset - bufferedOffset);
			glVertexArrayAttribBinding(m_VAO, i, element.ElementDivisor);

			/*
			glVertexAttribPointer(i + offset, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, m_Layout.GetStride(), (const void*)element.Offset);
			glEnableVertexAttribArray(i + offset);
			glVertexAttribDivisor(i + offset, m_Layout.GetDivisor());
			*/
		}

		if (info->Data) {
			SetData(info->Data, m_Size);
		}
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		glDeleteVertexArrays(1, &m_VAO);
	}
	void OpenGLVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer, uint32_t binding)
	{
		if (m_VAO)
		{
			if (m_VAO != s_BoundVAO) {
				glBindVertexArray(m_VAO);
				s_BoundVAO = m_VAO;
				s_CurrentLayout = m_Layout;
			}
		}
		glVertexArrayVertexBuffer(s_BoundVAO, binding, m_ID, 0, s_CurrentLayout.GetBufferStride(binding));
	}
	void OpenGLVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glNamedBufferSubData(m_ID, 0, size, data);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		glCreateBuffers(1, &m_ID);

		if (info->Data != nullptr)
			SetData(info->Data, info->Size);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
	void OpenGLIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void OpenGLIndexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void OpenGLIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;
		glNamedBufferData(m_ID, size, data, GL_STREAM_DRAW + m_Usage);
	}
	OpenGLUniformBuffer::OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Size(createInfo->Size),
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{
		m_Name = createInfo->Name;
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, m_Size, nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
	void OpenGLUniformBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void OpenGLUniformBuffer::Unbind()
	{

	}
	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_ID);
		glNamedBufferSubData(m_ID, 0, size, data);
	}
}
#endif

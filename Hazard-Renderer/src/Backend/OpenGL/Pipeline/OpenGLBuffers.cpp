
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

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(VertexBufferCreateInfo* info) : m_Size(info->Size)
	{
		ASSERT(info->Layout != nullptr, "Buffer layout cannot be nullptr");

		m_DebugName = info->DebugName;
		m_Usage = info->Usage;
		m_Layout = std::move(*info->Layout);

		glCreateVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, GL_STREAM_DRAW + m_Usage);

		for (uint32_t i = 0; i < m_Layout.GetElementCount(); i++) {
			BufferElement element = m_Layout.GetElements()[i];
			glVertexAttribPointer(i, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, m_Layout.GetStride(), (const void*)element.Offset);
			glEnableVertexAttribArray(i);
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
	void OpenGLVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}
	void OpenGLVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		m_Usage = info->Usage;

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);

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
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW + m_Usage);
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

#pragma once
#include <hzrpch.h>
#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;

		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, GL_STREAM_DRAW + m_Usage);

		if (info.Data) {
			SetData(info.Data, m_Size);
		}
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}
	void OpenGLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;
		glGenBuffers(1, &m_ID);
		Bind();

		if (info.Data != nullptr) 
			SetData(info.Data, info.Size);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void OpenGLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void OpenGLIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STREAM_DRAW + m_Usage);
	}
	OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferCreateInfo& createInfo)
	{

	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{

	}
	void OpenGLUniformBuffer::Bind()
	{

	}
	void OpenGLUniformBuffer::Unbind()
	{

	}
}
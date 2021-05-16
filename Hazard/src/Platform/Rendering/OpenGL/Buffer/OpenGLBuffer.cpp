#pragma once

#include <hzrpch.h>
#include "OpenGLBuffer.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBufferCreateInfo& info)
	{

		m_Info.dataStream = info.dataStream;
		m_Info.layout = *info.layout;

		glGenBuffers(1, &m_BufferID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, info.size, nullptr, GL_STREAM_DRAW + info.dataStream);

		if (info.data != nullptr) 
			SetData(info.data, info.size);

	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBufferCreateInfo& info)
	{
		m_Info.count = 0;
		m_Info.dataStream = info.dataStream;
		glGenBuffers(1, &m_BufferID);
		Bind();

		if (info.data != nullptr)
			SetData(info.data, info.size);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void OpenGLIndexBuffer::SetData(uint32_t* indices, uint32_t c)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, c * sizeof(uint32_t), indices, GL_STREAM_DRAW + m_Info.dataStream);
		this->m_Info.count = c;
	}
}
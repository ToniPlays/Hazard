#pragma once

#include <hzrpch.h>
#include "OpenGLBuffers.h"

namespace Hazard {

	OpenGLVertexBuffer::OpenGLVertexBuffer(ShaderDataType type, std::string name)
	{
		glGenBuffers(1, &BufferID);
		this->dataType = type;
		this->name = name;
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &BufferID);
	}

	void OpenGLVertexBuffer::SetData(void* data, uint32_t verticeCount)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, (double)verticeCount * ShaderDataTypeSize(dataType), data, GL_DYNAMIC_DRAW);
		dataLength = verticeCount;
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//---------------------- Index buffer ----------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer()
	{
		glGenBuffers(1, &BufferID);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &BufferID);
	}

	void OpenGLIndexBuffer::SetData(int* indices, uint32_t indiceCount)
	{
		Bind();
		this->count = indiceCount;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceCount * sizeof(int), indices, GL_DYNAMIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
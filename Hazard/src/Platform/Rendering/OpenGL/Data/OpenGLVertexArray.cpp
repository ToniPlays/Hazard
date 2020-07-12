#pragma once

#include <hzrpch.h>
#include "OpenGLVertexArray.h"

namespace Hazard {


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &ArrayID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &ArrayID);
	}

	void OpenGLVertexArray::BindAll() const
	{

		glBindVertexArray(ArrayID);

		int index = 0;
		for (VertexBuffer* buffer : layout) {

			buffer->Bind();
			int components = ComponentCount(buffer->GetType());
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, components, TypeToOpenGLType(buffer->GetType()), GL_FALSE, buffer->GetSize(), nullptr);
			index++;
		}

		if(indexBuffer != nullptr) 
			indexBuffer->Bind();
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(ArrayID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
		uint8_t index = 0;

		for (VertexBuffer* buffer : layout) {
			glDisableVertexAttribArray(index);
		}
		if (indexBuffer != nullptr) indexBuffer->Unbind();
	}

	void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
	{
		Bind(); 
		this->indexBuffer = indexBuffer; 
	};

	void OpenGLVertexArray::SetLayout(std::initializer_list<VertexBuffer*> buffers)
	{
		layout = buffers;
	}

	VertexBuffer* OpenGLVertexArray::GetBuffer(std::string name)
	{
		for (VertexBuffer* buffer : layout) {
			if (buffer->GetName() == name) return buffer;
		}
		return nullptr;
	}

}
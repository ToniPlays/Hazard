#include <hzrpch.h>
/*#pragma once

#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Hazard::Rendering::OpenGL
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

	OpenGLVertexArray::OpenGLVertexArray(const VertexArrayCreateInfo& info)
	{
		glCreateVertexArrays(1, &m_ID);
		AddBuffer(VertexBuffer::Create(*info.VertexBuffer));
		SetIndexBuffer(IndexBuffer::Create(*info.IndexBuffer));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}
	void OpenGLVertexArray::Bind()
	{
		

	}
	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddBuffer(VertexBuffer* buffer)
	{
		glBindVertexArray(m_ID);
		buffer->Bind();

		BufferLayout layout = buffer->GetLayout();

		for (const BufferElement& element : layout) {
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));

					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				HZR_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		m_Buffers.push_back(buffer);

	}
	void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* buffer)
	{
		Bind();
		buffer->Bind();
		this->m_IndexBuffer = buffer;
	}
}*/
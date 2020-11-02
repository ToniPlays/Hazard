#pragma once

#include <hzrpch.h>
#include "OpenGLVertexArray.h"

namespace Hazard {

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


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &ArrayID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &ArrayID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(ArrayID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::EnableAll(uint32_t index) const
	{
		Bind();
		vertexBuffers.at(index)->Bind();
		const BufferLayout& layout = vertexBuffers.at(index)->GetLayout();
		uint32_t offset = 0;

		for (uint16_t i = 0; i < layout.GetElements().size(); i++) {

			ShaderDataType type = layout.GetElements()[i].Type;
			glEnableVertexArrayAttrib(ArrayID, i);
			glVertexAttribPointer(i, ComponentCount(type), ShaderDataTypeToOpenGLBaseType(type),
				GL_FALSE, layout.GetStride(), (const void*)offset);

			offset += ShaderDataTypeSize(type);
		}
	}
	void OpenGLVertexArray::AddBuffer(VertexBuffer* vertexBuffer)
	{
		Bind();
		vertexBuffer->Bind();
		HZR_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
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
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(vertexBufferIndex, 1);
					vertexBufferIndex++;
				}
				break;
			}
			default:
				HZR_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		vertexBuffers.push_back(vertexBuffer);
	}
	IndexBuffer* OpenGLVertexArray::GetIndexBuffer() const
	{
		return indexBuffer;
	}
	void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		this->indexBuffer = indexBuffer;
	}
}
#pragma once
#include <hzrpch.h>
#include "OpenGLPipeline.h"

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


	OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& specs) : m_Specs(specs)
	{
		glCreateVertexArrays(1, &m_ID);

		m_Shader = Shader::Create(specs.ShaderPath);

		m_Buffer = VertexBuffer::Create(*specs.pVertexBuffer);
		m_IndexBuffer = IndexBuffer::Create(*specs.pIndexBuffer);
		
		Invalidate();
	}
	OpenGLPipeline::~OpenGLPipeline()
	{
		glDeleteVertexArrays(1, &m_ID);
	}
	void OpenGLPipeline::Invalidate()
	{

	}
	void OpenGLPipeline::Bind()
	{
		glBindVertexArray(m_ID);
		m_Shader->Bind();
		glBindVertexArray(m_ID);
		m_Buffer->Bind();
		BufferLayout layout = m_Buffer->GetLayout();
		uint32_t offset = 0;

		for (uint16_t i = 0; i < layout.GetElements().size(); i++) {

			ShaderDataType type = layout.GetElements()[i].Type;
			glEnableVertexArrayAttrib(m_ID, i);
			glVertexAttribPointer(i, ComponentCount(type), ShaderDataTypeToOpenGLBaseType(type),
				layout.GetElements()[i].Normalized, layout.GetStride(), (const void*)offset);

			offset += ShaderDataTypeSize(type);
		}
		if (m_IndexBuffer)
			m_IndexBuffer->Bind();

		glDrawArrays(GL_TRIANGLES, 0, m_IndexBuffer->GetCount());
	}
	void OpenGLPipeline::Draw(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}
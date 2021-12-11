#pragma once
#include <hzrpch.h>
#include "OpenGLPipeline.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLUtils.h"
#include <glad/glad.h>

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
		HZR_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_ID);

		m_Shader = Shader::Create(specs.ShaderPath);
		Invalidate();
	}
	OpenGLPipeline::~OpenGLPipeline()
	{
		glDeleteVertexArrays(1, &m_ID);
	}
	void OpenGLPipeline::Invalidate()
	{
		m_DrawType = OpenGLUtils::DrawTypeToGLType(m_Specs.DrawType);
	}
	void OpenGLPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
	{
		m_Shader->Bind();
		glBindVertexArray(m_ID);
		const ShaderStageData& data = m_Shader->GetShaderData().Stages.at(ShaderType::Vertex);

		for (auto& [location, input] : data.Inputs) {
			glEnableVertexArrayAttrib(m_ID, location);
			ShaderDataType type = input.Type;
			glVertexAttribPointer(location, ComponentCount(type), ShaderDataTypeToOpenGLBaseType(type),
				GL_FALSE, data.Stride, (const void*)input.Offset);
		}
	}
	void OpenGLPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		commandBuffer->GetStats().DrawCalls++;
		commandBuffer->GetStats().IndexCount += count;

		glLineWidth(m_Specs.LineWidth);
		glDrawElements(m_DrawType, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		commandBuffer->GetStats().DrawCalls++;

		glLineWidth(m_Specs.LineWidth);
		glDrawArrays(m_DrawType, 0, count);
	}
}
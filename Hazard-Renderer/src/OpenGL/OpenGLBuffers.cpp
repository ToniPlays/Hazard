
#include "OpenGLBuffers.h"

#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Renderer.h"
#include "Backend/Core/ShaderDataType.h"
#include "OpenGLContext.h"
#include "OpenGLSwapchain.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	/*
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
		HZR_PROFILE_FUNCTION();
		m_DebugName = info->Name;

		if (info->Layout != nullptr)
			m_Layout = *info->Layout;
		
		if (info->Data)
			m_LocalBuffer = Buffer::Copy(info->Data, info->Size);

		uint32_t offset = 0;
		Ref<OpenGLVertexBuffer> instance = this;

		Renderer::SubmitResourceCreate([instance]() mutable {

			if (instance->m_Layout.GetStride() == 0)
			{
				glCreateBuffers(1, &instance->m_BufferID);
				glNamedBufferData(instance->m_BufferID, instance->m_Size, nullptr, GL_STATIC_DRAW);

				if (instance->m_LocalBuffer)
				{
					BufferCopyRegion region = {};
					region.Data = instance->m_LocalBuffer.Data;
					region.Size = instance->m_LocalBuffer.Size;
					region.Offset = 0;

					instance->SetData_RT(region);
				}

				return;
			}

			glCreateVertexArrays(1, &instance->m_VAO);
			glCreateBuffers(1, &instance->m_BufferID);
			glNamedBufferData(instance->m_BufferID, instance->m_Size, nullptr, GL_STATIC_DRAW);

			uint32_t bufferedOffset = 0;
			uint32_t currentBuffer = 0;

			for (uint32_t i = 0; i < instance->m_Layout.GetElementCount(); i++)
			{
				BufferElement element = instance->m_Layout.GetElements()[i];

				uint32_t stride = instance->m_Layout.GetBufferStride(element.ElementDivisor);

				if (element.ElementDivisor != currentBuffer)
				{
					bufferedOffset += element.Offset;
					currentBuffer = element.ElementDivisor;
					glVertexArrayBindingDivisor(instance->m_VAO, element.ElementDivisor, element.ElementDivisor);
				}

				glEnableVertexArrayAttrib(instance->m_VAO, i);
				glVertexArrayAttribFormat(instance->m_VAO, i, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), GL_FALSE, element.Offset - bufferedOffset);
				glVertexArrayAttribBinding(instance->m_VAO, i, element.ElementDivisor);
			}
			if (instance->m_LocalBuffer)
			{
				BufferCopyRegion region = {};
				region.Data = instance->m_LocalBuffer.Data;
				region.Size = instance->m_LocalBuffer.Size;
				region.Offset = 0;

				instance->SetData_RT(region);
			}
		});
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		HZR_PROFILE_FUNCTION();
		Renderer::SubmitResourceFree([id = m_BufferID, vao = m_VAO]() mutable {
			glDeleteBuffers(1, &id);
			glDeleteVertexArrays(1, &vao);
			});
	}
	void OpenGLVertexBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		m_LocalBuffer.Release();
		m_LocalBuffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, copyRegion]() mutable {
			instance->SetData_RT(copyRegion);
			});
	}
	void OpenGLVertexBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		glNamedBufferSubData(m_BufferID, copyRegion.Offset, copyRegion.Size, m_LocalBuffer.Data);
		m_LocalBuffer.Release();
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		HZR_PROFILE_FUNCTION();
		m_DebugName = info->Name;

		Ref<OpenGLIndexBuffer> instance = this;

		if (info->Data)
			m_LocalBuffer = Buffer::Copy(info->Data, m_Size);

		Renderer::SubmitResourceCreate([instance]() mutable {

			glCreateBuffers(1, &instance->m_BufferID);
			glNamedBufferData(instance->GetBufferID(), instance->m_Size, nullptr, GL_STATIC_DRAW);

			if (instance->m_LocalBuffer.Data != nullptr)
			{
				BufferCopyRegion copyRegion = {};
				copyRegion.Data = instance->m_LocalBuffer.Data;
				copyRegion.Size = instance->m_LocalBuffer.Size;
				copyRegion.Offset = 0;

				instance->SetData_RT(copyRegion);
			}
			});
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZR_PROFILE_FUNCTION();
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::SubmitResourceFree([instance]() mutable {
			glDeleteBuffers(1, &instance->m_BufferID);
			});
	}
	void OpenGLIndexBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();

		m_LocalBuffer.Release();
		m_LocalBuffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance, copyRegion]() mutable {
			instance->SetData_RT(copyRegion);
			});
	}
	void OpenGLIndexBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		glNamedBufferSubData(m_BufferID, copyRegion.Offset, copyRegion.Size, m_LocalBuffer.Data);
		m_LocalBuffer.Release();
	}
	OpenGLUniformBuffer::OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Name(createInfo->Name), m_Size(createInfo->Size),
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{
		HZR_PROFILE_FUNCTION();
		m_LocalData.Allocate(m_Size * 32);
		m_LocalData.ZeroInitialize();

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			glCreateBuffers(1, &instance->m_BufferID);
			glNamedBufferData(instance->m_BufferID, instance->m_LocalData.Size, nullptr, GL_DYNAMIC_DRAW);
			});
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		HZR_PROFILE_FUNCTION();
		m_LocalData.Release();
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			glDeleteBuffers(1, &instance->m_BufferID);
			});
	}
	void OpenGLUniformBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();

		uint32_t frameIndex = OpenGLContext::GetInstance().GetSwapchain().As<OpenGLSwapchain>()->GetFrameIndex();

		if (m_FrameIndex != frameIndex)
		{
			m_CurrentBufferDataIndex = 0;
			m_FrameIndex = frameIndex;
		}

		m_LocalData.Write(copyRegion.Data, copyRegion.Size, m_CurrentBufferDataIndex);
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, startIndex = m_CurrentBufferDataIndex, size = copyRegion.Size]() mutable {

			HZR_PROFILE_FUNCTION("OpenGLUniformBuffer::SetData(const void*, uint32_t)_RT");
			glNamedBufferData(instance->m_BufferID, size, (uint8_t*)instance->m_LocalData.Data + startIndex, GL_DYNAMIC_DRAW);
			});
		m_CurrentBufferDataIndex += m_Size;
	}
	*/
}
#endif

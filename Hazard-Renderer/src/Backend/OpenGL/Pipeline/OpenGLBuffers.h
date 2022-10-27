#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Pipeline/Buffers.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferCreateInfo* createInfo);
		~OpenGLVertexBuffer();

		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);
		size_t GetSize() override { return m_Size; }
		std::string& GetDebugName() { return m_DebugName; };
		const BufferLayout& GetLayout() const override { return m_Layout; }

		//OpenGL specific
		uint32_t GetVAOID() const { return m_VAO; }
		uint32_t GetBufferID() const { return m_BufferID; }

	private:
		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_BufferID = 0, m_VAO = 0;
		uint32_t m_Size;
		BufferLayout m_Layout;
		Buffer m_LocalBuffer;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(IndexBufferCreateInfo* createInfo);
		~OpenGLIndexBuffer();

		size_t GetCount() override { return m_Size / sizeof(uint32_t); }
		std::string& GetDebugName() { return m_DebugName; };
		//OpenGL specific
		uint32_t GetBufferID() { return m_BufferID; }

	private:
		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);

		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_BufferID;
		uint32_t m_Size;
		Buffer m_LocalBuffer;

	};
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo);
		~OpenGLUniformBuffer();

		void SetData(const void* data, size_t size) override;

		std::string& GetName() { return m_Name; }
		const uint32_t GetBinding() const override { return m_Binding; };
		const size_t GetSize() const override { return m_Size; };

		uint32_t GetUsageFlags() { return m_Usage; };

		//OpenGL specific
		uint32_t GetBufferID() const { return m_BufferID; }

	private:
		std::string m_Name;
		uint32_t m_BufferID;
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;

		Buffer m_LocalData;
		uint32_t m_CurrentBufferDataIndex = 0;
		uint32_t m_FrameIndex = 255;
	};
}
#endif

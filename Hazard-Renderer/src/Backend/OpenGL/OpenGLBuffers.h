#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Buffers.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferCreateInfo* createInfo);
		~OpenGLVertexBuffer();

		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);
		const size_t GetSize() const override { return m_Size; }
		std::string& GetDebugName() { return m_DebugName; };
		const BufferLayout& GetLayout() const override { return m_Layout; }

		//OpenGL specific
		uint32_t GetVAOID() const { return m_VAO; }
		uint32_t GetBufferID() const { return m_BufferID; }

	private:
		std::string m_DebugName;
		uint32_t m_BufferID = 0, m_VAO = UINT32_MAX;
		uint32_t m_Size;
		BufferLayout m_Layout;
		Buffer m_LocalBuffer;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(IndexBufferCreateInfo* createInfo);
		~OpenGLIndexBuffer();

		size_t GetCount() const override { return m_Size / sizeof(uint32_t); }
		std::string& GetDebugName() { return m_DebugName; };
		const size_t GetSize() const { return m_Size; };

		//OpenGL specific
		uint32_t GetBufferID() { return m_BufferID; }

	private:
		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);

		std::string m_DebugName;
		uint32_t m_BufferID;
		uint32_t m_Size;
		Buffer m_LocalBuffer;

	};
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo);
		~OpenGLUniformBuffer();

		void SetData(const BufferCopyRegion& copyRegion) override;

		std::string& GetName() { return m_Name; }
		const uint32_t GetBinding() const override { return m_Binding; };
		const size_t GetSize() const override { return m_Size; };

		uint32_t GetUsageFlags() { return m_Usage; };
		void AddUsageFlags(uint32_t flags) override { m_Usage |= flags; };

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

#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Pipeline/Buffers.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::OpenGL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferCreateInfo* createInfo);
		~OpenGLVertexBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void SetData(const void* data, uint32_t size) override;
		uint32_t GetSize() override { return m_Size; }
		std::string& GetDebugName() { return m_DebugName; };
		const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_ID, m_VAO;
		uint32_t m_Size;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(IndexBufferCreateInfo* createInfo);
		~OpenGLIndexBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override;

		uint32_t GetCount() override { return m_Size / sizeof(uint32_t); }
		std::string& GetDebugName() { return m_DebugName; };

	private:
		void SetData(uint32_t* data, uint32_t size) override;

		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_ID;
		uint32_t m_Size;

	};
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo);
		~OpenGLUniformBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind() override;
		void SetData(const void* data, uint32_t size) override;

		std::string& GetName() { return m_Name; }
		const uint32_t GetBinding() const override { return m_Binding; };
		const uint32_t GetSize() const override { return m_Size; };

		uint32_t GetUsageFlags() { return m_Usage; };

	private:
		std::string m_Name;
		uint32_t m_ID;
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;
	};
}
#endif

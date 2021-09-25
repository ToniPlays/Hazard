#pragma once

#include "Hazard/Rendering/Buffers/VertexArray.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray(VertexArrayCreateInfo* info);
		~OpenGLVertexArray();
		
		void Bind() override;
		void Unbind() override;
		void AddBuffer(VertexBuffer* buffer) override;
		virtual VertexBuffer& GetVertexBuffer() override { return *m_Buffers[0]; }

		IndexBuffer& GetIndexBuffer() override {return *m_IndexBuffer; }
		void SetIndexBuffer(IndexBuffer* buffer) override;
	private:
		std::vector<VertexBuffer*> m_Buffers;
		IndexBuffer* m_IndexBuffer;

		uint32_t m_ID = 0;
		uint32_t m_VertexBufferIndex = 0;

	};
}
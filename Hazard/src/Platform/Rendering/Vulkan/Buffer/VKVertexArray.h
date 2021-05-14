#pragma once

#include "Hazard/Rendering/Buffers/VertexArray.h"

namespace Hazard::Rendering::Vulkan {

	class VKVertexArray : public VertexArray {
	public:
		VKVertexArray(VertexArrayCreateInfo info);
		~VKVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void EnableAll(uint32_t index) const override;
		uint32_t GetID() const override { return m_ID; }

		void AddBuffer(VertexBuffer* buffer) { m_Buffers.push_back(buffer); }
		IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
		std::vector<VertexBuffer*> GetBuffers() override { return m_Buffers; }
		void SetIndexBuffer(IndexBuffer* indexBuffer) override { m_IndexBuffer = indexBuffer; }

	private:
		uint32_t m_ID;
		std::vector<VertexBuffer*> m_Buffers;
		IndexBuffer* m_IndexBuffer;
	};
}
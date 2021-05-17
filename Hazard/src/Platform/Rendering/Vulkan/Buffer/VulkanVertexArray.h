#pragma once

#include "Hazard/Rendering/Buffers/VertexArray.h"

namespace Hazard::Rendering::Vulkan 
{
	class VulkanVertexArray : public VertexArray {

	public:
		VulkanVertexArray(const VertexArrayCreateInfo& info);
		~VulkanVertexArray();

		void Bind() const;
		void Unbind() const;
		void EnableAll(uint32_t index = 0) const ;
		void AddBuffer(VertexBuffer* vertexBuffer) {};

		IndexBuffer* GetIndexBuffer() const { return nullptr; };
		void SetIndexBuffer(IndexBuffer* indexBuffer) { };

		uint32_t GetID() const { return 0; };
		std::vector<VertexBuffer*> GetBuffers() { return m_Buffers;	}

	private:
		std::vector<VertexBuffer*> m_Buffers;
		IndexBuffer* m_IndexBuffer;
	};
}
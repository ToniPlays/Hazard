#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Renderer/Buffers/VertexArray.h"
#include "Hazard/Modules/Renderer/Buffers/Buffer.h"
#include "glad/glad.h"


namespace Hazard {
	class HAZARD_API OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		virtual IndexBuffer* GetIndexBuffer() const { return indexBuffer; }
		void SetIndexBuffer(IndexBuffer* indexBuffer) { Bind(); this->indexBuffer = indexBuffer; this->indexBuffer->Bind(); };
		void SetLayout(std::initializer_list<VertexBuffer*> buffers);
		void AddBuffer(VertexBuffer* buffer) { layout.push_back(buffer); };

	private:
		std::vector<VertexBuffer*> layout;
		IndexBuffer* indexBuffer = nullptr;
		uint32_t ArrayID;
	};
}
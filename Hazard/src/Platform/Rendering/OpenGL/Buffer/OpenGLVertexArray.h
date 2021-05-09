#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Buffers/Buffer.h"
#include "Hazard/Rendering/Buffers/VertexArray.h"
#include "glad/glad.h"


namespace Hazard::Rendering::OpenGL {
	class  OpenGLVertexArray : public VertexArray {
	public:

		OpenGLVertexArray();
		~OpenGLVertexArray();
		void Bind() const;
		void Unbind() const;
		void EnableAll(uint32_t index) const override;
		uint32_t GetID() const { return ArrayID; }

		void AddBuffer(VertexBuffer* vertexBuffer) override;
		IndexBuffer* GetIndexBuffer() const;
		std::vector<VertexBuffer*> GetBuffers() override { return vertexBuffers; }
		void SetIndexBuffer(IndexBuffer* indexBuffer) override;

	private:
		uint32_t ArrayID;
		uint32_t vertexBufferIndex = 0;
		std::vector<VertexBuffer*> vertexBuffers;
		IndexBuffer* indexBuffer;
	};
}
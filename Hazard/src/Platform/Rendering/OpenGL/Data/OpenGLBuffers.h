#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Rendering/Buffers/Buffer.h"

namespace Hazard {

	class HAZARD_API OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(ShaderDataType type, std::string name);
		~OpenGLVertexBuffer();

		void SetData(void* data, uint32_t size);
		void Bind() const;
		void Unbind() const;

		std::string GetName() const { return name; };
		uint32_t GetID() const { return BufferID; }
		uint32_t GetSize() const { return ShaderDataTypeSize(dataType); }
		ShaderDataType GetType() const { return dataType; };

	private:
		std::string name;
		ShaderDataType dataType = ShaderDataType::None;
		uint32_t BufferID;
	};

	class HAZARD_API OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer();
		~OpenGLIndexBuffer();

		void SetData(int* indices, uint32_t size);
		void Bind() const;
		void Unbind() const;
		uint32_t GetCount() const { return count; };
		uint32_t GetID() const { return BufferID; }
	private:
		uint32_t BufferID;
		uint32_t count = 0;
	};
}
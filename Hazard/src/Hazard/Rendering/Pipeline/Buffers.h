#pragma once
#include "Hazard/Core/Core.h"
#include "ShaderDataType.h"

namespace Hazard::Rendering 
{
	enum BufferUsage {
		StreamDraw = 0,
		StreamRead = 1,
		StreamCopy = 2,
		StaticDraw = 4,
		StaticRead = 5,
		StaticCopy = 6,
		DynamicDraw = 8,
		DynamicRead = 9,
		DynamicCopy = 10
	};


	struct VertexBufferCreateInfo 
	{
		uint32_t Size = 0;
		BufferUsage Usage = BufferUsage::StaticDraw;
		void* Data = nullptr;
	};
	struct IndexBufferCreateInfo 
	{
		uint32_t Size;
		BufferUsage Usage;
		uint32_t* Data = nullptr;
	};
	struct UniformBufferCreateInfo 
	{
		std::string Name;
		uint32_t Size;
		uint32_t Binding;
		uint32_t Usage;
	};


	struct Buffer {

		void* Data;
		uint32_t Size;

		Buffer() : Data(nullptr), Size(0) {};
		Buffer(void* data, uint32_t size) : Data(data), Size(size) {};

		void Allocate(uint32_t size) {
			delete[] Data;
			Data = nullptr;

			if (size == 0) return;

			Data = new byte[size];
			Size = size;
		}
		void Release() {
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}
		void ZeroInitialize() {
			if (Data) memset(Data, 0, Size);
		}
		template<typename T>
		T& Read(uint32_t offset = 0) {
			return *(T*)((byte*)Data + offset);
		}
		byte* ReadBytes(uint32_t size, uint32_t offset) {
			HZR_CORE_ASSERT(offset + size <= Size, "Vulkan Buffer Overflow");
			byte* buffer = new byte[size];
			memcpy(buffer, (byte*)Data + offset, size);
			return buffer;
		}
		void Write(void* data, uint32_t size, uint32_t offset = 0) {
			HZR_CORE_ASSERT(offset + size <= Size, "Vulkan Buffer Overflow");
			memcpy((byte*)Data + offset, data, size);
		}
		operator bool() const { return Data; }
		byte& operator[](int index) { return ((byte*)Data)[index]; }
		byte operator[](int index) const { return ((byte*)Data)[index]; }
		template<typename T>
		T* As() { return (T*)Data; }
		inline uint32_t GetSize() {	return Size; }

		static Buffer Copy(const void* data, uint32_t size) {
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}
	};

	class VertexBuffer : public RefCount
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(VertexBufferCreateInfo* createInfo);
	};

	class IndexBuffer : public RefCount
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(uint32_t* data, uint32_t size) = 0;
		virtual uint32_t GetCount() = 0;

		static Ref<IndexBuffer> Create(IndexBufferCreateInfo* createInfo);
	};
	class UniformBuffer : public RefCount
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data) = 0;
		virtual uint32_t GetUsageFlags() = 0;
		virtual const uint32_t GetSize() const = 0;

		static Ref<UniformBuffer> Create(UniformBufferCreateInfo* createInfo);
	};
}
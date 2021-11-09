#pragma once
namespace Hazard 
{
	struct Buffer {

		void* Data = nullptr;
		uint32_t Size = 0;

		Buffer() : Data(nullptr), Size(0) {};
		Buffer(void* data, uint32_t size) : Data(data), Size(size) {};

		void Allocate(uint32_t size) 
		{
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
		inline uint32_t GetSize() { return Size; }

		static Buffer Copy(const void* data, uint32_t size, uint32_t offset = 0) {
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, (byte*)data + offset, size);
			return buffer;
		}

	};
}
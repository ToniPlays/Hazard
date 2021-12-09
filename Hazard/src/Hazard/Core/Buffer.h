#pragma once

namespace Hazard 
{
	struct Buffer {

		uint32_t Size = 0;
		void* Data = nullptr;

		Buffer() : Data(nullptr), Size(0) {};
		Buffer(void* data, uint32_t size) : Data(data), Size(size) {};

		void Allocate(uint32_t size) 
		{
			if(Data != nullptr)
				delete[] Data;

			Data = nullptr;

			if (size == 0) return;

			Data = new byte[size];
			Size = size;
		}
		void Release() 
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}
		void ZeroInitialize() {
			if (Data) memset(Data, 0, Size);
		}
		template<typename T>
		void Initialize(T value) {
			if (Data) 
				memset(Data, value, Size);
		}
		template<typename T>
		T& Read(uint32_t offset = 0) {
			return *(T*)((byte*)Data + offset);
		}
		byte* ReadBytes(uint32_t size, uint32_t offset) {
			//HZR_CORE_ASSERT(offset + size <= Size, "Buffer Overflow");
			byte* buffer = new byte[size];
			memcpy(buffer, (byte*)Data + offset, size);
			return buffer;
		}
		void Write(void* data, uint32_t size, uint32_t offset = 0) {
			//HZR_CORE_ASSERT(offset + size <= Size, "Buffer Overflow");
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
		template<typename T>
		static T Get(void* data, uint32_t startIndex = 0) {
			T value;
			memcpy(&value, (byte*)data + startIndex, sizeof(T));
			return value;
		}
		template<typename T>
		static T GetRaw(void* data, uint32_t len, uint32_t startIndex = 0) {
			T value;
			memcpy(&value, data, len);
			return value;
		}
	};
}
#pragma once

#include <assert.h>
#include <iostream>

struct Buffer {

	uint32_t Size = 0;
	void* Data = nullptr;

	Buffer() : Data(nullptr), Size(0) {};
	Buffer(void* data, uint32_t size) : Data(data), Size(size) {};

	void Allocate(uint32_t size)
	{
		delete Data;
		Data = nullptr;

		if (size == 0)
			return;

		Data = new uint8_t[size];
		Size = size;

	}
	void Release()
	{
		delete Data;
		Data = nullptr;
		Size = 0;

	}
	void ZeroInitialize() {
		if (Data)
			memset(Data, 0, Size);
	}
	template<typename T>
	void Initialize(T value) {
		if (Data)
			memset(Data, value, Size);
	}
	template<typename T>
	T& Read(uint32_t offset = 0) {
		return *(T*)((uint8_t*)Data + offset);
	}
	uint8_t* ReadBytes(uint32_t size, uint32_t offset) {
		if (offset + size > Size) assert(false);
		uint8_t* buffer = new uint8_t[size];

		memcpy(buffer, (uint8_t*)Data + offset, size);
		return buffer;
	}
	void Write(void* data, uint32_t size, uint32_t offset = 0) {
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}
	void Write(const void* data, uint32_t size, uint32_t offset = 0) {
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}
	operator bool() const { return Data; }
	uint8_t& operator[](int index) { return ((uint8_t*)Data)[index]; }
	uint8_t operator[](int index) const { return ((uint8_t*)Data)[index]; }
	template<typename T>
	T* As() { return (T*)Data; }
	inline uint32_t GetSize() { return Size; }

	static Buffer Copy(const void* data, uint32_t size, uint32_t offset = 0) {
		Buffer buffer;
		buffer.Allocate(size);
		memcpy(buffer.Data, (uint8_t*)data + offset, size);
		return buffer;
	}
	template<typename T>
	static T Get(void* data, uint32_t startIndex = 0) {
		T value;
		memcpy(&value, (uint8_t*)data + startIndex, sizeof(T));
		return value;
	}
	template<typename T>
	static T GetRaw(void* data, uint32_t len, uint32_t startIndex = 0) {
		T value;
		memcpy(&value, data, len);
		return value;
	}
};

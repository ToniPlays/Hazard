#pragma once

#include "Profiling/MemoryDiagnostic.h"
#include <assert.h>

struct Buffer 
{
	size_t Size = 0;
	void* Data = nullptr;

	Buffer() : Data(nullptr), Size(0) {};
	Buffer(void* data, size_t size) : Data(data), Size(size) {};

	void Allocate(size_t size)
	{
		hdelete[] Data;
		Data = nullptr;

		if (size == 0)
			return;
        
		Data = hnew uint8_t[size];
		Size = size;
	}
	void Release()
	{
        hdelete[] Data;
		Data = nullptr;
		Size = 0;
	}
	void ZeroInitialize() 
	{
		if (Data)
			memset(Data, 0, Size);
	}
	template<typename T>
	void Initialize(T value) 
	{
		if (Data)
			memset(Data, value, Size);
	}
	template<typename T>
	T& Read(size_t offset = 0) 
	{
		return *(T*)((uint8_t*)Data + offset);
	}
	uint8_t* ReadBytes(size_t size, size_t offset) 
	{
		if (offset + size > Size) assert(false);
		uint8_t* buffer = hnew uint8_t[size];

		memcpy(buffer, (uint8_t*)Data + offset, size);
		return buffer;
	}
	void Write(void* data, size_t size, size_t offset = 0) 
	{
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}
	void Write(const void* data, size_t size, size_t offset = 0) 
	{
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}
	bool TryWrite(const void* data, size_t size, size_t offset = 0)
	{
		if (offset + size > Size) return false;
		memcpy((uint8_t*)Data + offset, data, size);
		return true;
	}
	operator bool() const { return Data; }
	uint8_t& operator[](int index) { return ((uint8_t*)Data)[index]; }
	uint8_t operator[](int index) const { return ((uint8_t*)Data)[index]; }
	template<typename T>
	T* As() { return (T*)Data; }
	inline size_t GetSize() { return Size; }

	static Buffer Copy(const void* data, size_t size, size_t offset = 0) 
	{
		Buffer buffer;
		buffer.Allocate(size);
		memcpy(buffer.Data, (uint8_t*)data + offset, size);
		return buffer;
	}
	static Buffer Copy(const Buffer& source)
	{
		return Copy(source.Data, source.Size);
	}

	template<typename T>
	static T Get(void* data, size_t startIndex = 0) {
		T value = {};
		memcpy(&value, (uint8_t*)data + startIndex, sizeof(T));
		return value;
	}
	template<typename T>
	static T GetRaw(void* data, size_t len, size_t startIndex = 0) {
		T value = {};
		memcpy(&value, data, len);
		return value;
	}
};

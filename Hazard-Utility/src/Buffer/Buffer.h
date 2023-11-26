#pragma once

#include "Profiling/MemoryDiagnostic.h"
#include <assert.h>

struct Buffer 
{
	uint64_t Size = 0;
	void* Data = nullptr;

	Buffer() : Data(nullptr), Size(0) {};
	Buffer(void* data, uint64_t size) : Data(data), Size(size) {};

	void Allocate(uint64_t size)
    {
        if(Data)
            hdelete[] (uint8_t*)Data;
        
        Data = nullptr;
        
		if (size == 0)
			return;
        
		Data = new uint8_t[size];
		Size = size;
	}

	void Release()
	{
        if(Data)
            hdelete[] (uint8_t*)Data;
        
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
	T Read(uint64_t offset = 0) 
	{
		if(sizeof(T) + offset > Size) assert(false);
		return *(T*)((uint8_t*)Data + offset);
	}

	template<>
	std::string Read(uint64_t offset)
	{
		uint64_t length = Read<uint64_t>(offset);
		return std::string((char*)Data + offset + sizeof(uint64_t), length);
	}

    void* Read(uint64_t size, uint64_t offset = 0)
    {
        return ((uint8_t*)Data + offset);
    }

	uint8_t* ReadBytes(uint64_t size, uint64_t offset) 
	{
		if (offset + size > Size) assert(false);
        
		uint8_t* buffer = hnew uint8_t[size];
		memcpy(buffer, (uint8_t*)Data + offset, size);
		return buffer;
	}

	void Write(void* data, uint64_t size, uint64_t offset = 0) 
	{
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}

	void Write(const void* data, uint64_t size, uint64_t offset = 0) 
	{
		if (offset + size > Size) assert(false);
		memcpy((uint8_t*)Data + offset, data, size);
	}

	void Write(const std::string& data, uint64_t offset = 0)
	{
		uint64_t length = data.length();
		memcpy((uint8_t*)Data + offset, &length, sizeof(uint64_t));
		memcpy((uint8_t*)Data + offset + sizeof(uint64_t), (uint8_t*)data.c_str(), length);
	}

	bool TryWrite(const void* data, uint64_t size, uint64_t offset = 0)
	{
		if (offset + size > Size) return false;
		memcpy((uint8_t*)Data + offset, data, size);
		return true;
	}

	operator bool() const { return Data; }
	uint8_t& operator[](int index) { return ((uint8_t*)Data)[index]; }
	uint8_t operator[](int index) const { return ((uint8_t*)Data)[index]; }

	inline uint64_t GetSize() { return Size; }

	static Buffer Copy(const void* data, uint64_t size, uint64_t offset = 0) 
	{
		Buffer buffer;
		buffer.Allocate(size);
		buffer.Write((uint8_t*)data + offset, size);
		return buffer;
	}
	static Buffer Copy(const Buffer& source)
	{
		return Copy(source.Data, source.Size);
	}

	template<typename T>
	static T Get(void* data, uint64_t startIndex = 0) 
	{
		T value = {};
		memcpy(&value, (uint8_t*)data + startIndex, sizeof(T));
		return value;
	}

	template<typename T>
	static T GetRaw(void* data, uint64_t len, uint64_t startIndex = 0) 
	{
		T value = {};
		memcpy(&value, data, len);
		return value;
	}
};

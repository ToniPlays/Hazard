#pragma once

#include "Buffer.h"
#include "UtilityCore.h"
#include "MemoryDiagnostic.h"

class CachedBuffer
{
public:
	CachedBuffer(size_t size)
	{
		m_DataBuffer.Allocate(size);
		m_DataBuffer.ZeroInitialize();
	}

	CachedBuffer(const CachedBuffer& other)
		: m_DataBuffer(other.m_DataBuffer), m_CurrentBufferOffset(other.m_CurrentBufferOffset)
	{
	}

	CachedBuffer(CachedBuffer&& other)
	{
		m_DataBuffer = other.m_DataBuffer;
		m_CurrentBufferOffset = other.m_CurrentBufferOffset;

		other.m_DataBuffer = Buffer();
		other.m_CurrentBufferOffset = 0;
	}

	~CachedBuffer()
	{
		m_DataBuffer.Release();
	}

	CachedBuffer& operator=(std::nullptr_t)
	{
		m_DataBuffer = Buffer();
		m_CurrentBufferOffset = 0;
		return *this;
	}

	CachedBuffer& operator=(const CachedBuffer& other)
	{
		m_DataBuffer = other.m_DataBuffer;
		m_CurrentBufferOffset = other.m_CurrentBufferOffset;
		return *this;
	}
	CachedBuffer& operator=(CachedBuffer&& other)
	{
		m_DataBuffer = other.m_DataBuffer;
		m_CurrentBufferOffset = other.m_CurrentBufferOffset;

		other.m_DataBuffer = Buffer();
		other.m_CurrentBufferOffset = 0;

		return *this;
	}

	template<typename T>
	T Read()
	{
		T value = Buffer::Get<T>(m_DataBuffer.Data, m_CurrentBufferOffset);
		m_CurrentBufferOffset += sizeof(T);
		return value;
	}

	template<typename T>
	T Read(uint32_t size)
	{
		uint32_t* start = (uint32_t*)m_DataBuffer.Data + m_CurrentBufferOffset;
		m_CurrentBufferOffset += size;
		return std::vector<uint32_t>(start, start + size);
	}

	template<>
	Buffer Read(uint32_t size)
	{
		Buffer value((uint8_t*)m_DataBuffer.Data + m_CurrentBufferOffset, size);
		m_CurrentBufferOffset += size;
		return value;
	}

	template<typename T>
	size_t Write(T value)
	{
		m_DataBuffer.Write(&value, sizeof(T), m_CurrentBufferOffset);
		m_CurrentBufferOffset += sizeof(T);
		return sizeof(T);
	}
	template<>
	size_t Write(Buffer value)
	{
		m_DataBuffer.Write(value.Data, value.Size, m_CurrentBufferOffset);
		m_CurrentBufferOffset += value.Size;
		return value.Size;
	}

	template<typename T>
	size_t Write(T* data, uint32_t dataLength)
	{
		m_DataBuffer.Write((const void*)data, dataLength, m_CurrentBufferOffset);
		m_CurrentBufferOffset += dataLength;
		return dataLength;
	}

	bool Available() const { return m_CurrentBufferOffset < m_DataBuffer.Size; }

	void* GetData() const { return m_DataBuffer.Data; }
	size_t GetSize() { return m_DataBuffer.Size; }
	void SetBufferOffsfet(size_t offset) { m_CurrentBufferOffset = offset; }

private:

private:
	Buffer m_DataBuffer;
	size_t m_CurrentBufferOffset = 0;
	uint32_t m_RefCount = 0;

};
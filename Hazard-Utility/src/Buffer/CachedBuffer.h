#pragma once

#include "Buffer.h"
#include "UtilityCore.h"
#include "Profiling/MemoryDiagnostic.h"

class CachedBuffer
{
public:

	CachedBuffer() = default;

	CachedBuffer(size_t size)
	{
		m_DataBuffer.Allocate(size);
		m_DataBuffer.ZeroInitialize();
	}
	CachedBuffer(void* data, size_t size)
	{
		m_DataBuffer = Buffer(data, size);
		m_OwnsData = false;
	}

	CachedBuffer(const CachedBuffer& other)
		: m_DataBuffer(other.m_DataBuffer), m_CurrentBufferOffset(other.m_CurrentBufferOffset) {}

	CachedBuffer(CachedBuffer&& other) noexcept
	{
		m_DataBuffer = other.m_DataBuffer;
		m_CurrentBufferOffset = other.m_CurrentBufferOffset;

		other.m_DataBuffer = Buffer();
		other.m_CurrentBufferOffset = 0;
	}

	~CachedBuffer()
	{
		if (m_OwnsData)
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
	CachedBuffer& operator=(CachedBuffer&& other) noexcept
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
	template<>
	std::string Read()
	{
		uint64_t length = Read<uint64_t>();
		Buffer data = Read<Buffer>(length);
		return std::string((char*)data.Data, length);
	}

	template<typename T>
	T Read(size_t size)
	{
		uint32_t* start = (uint32_t*)m_DataBuffer.Data + m_CurrentBufferOffset;
		m_CurrentBufferOffset += size;
		return std::vector<uint32_t>(start, start + size);
	}

	template<>
	Buffer Read(size_t size)
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
	template<>
	size_t Write(std::string value)
	{
		uint64_t length = value.length();
		m_DataBuffer.Write(&length, sizeof(uint64_t), m_CurrentBufferOffset);
		m_CurrentBufferOffset += sizeof(uint64_t);

		m_DataBuffer.Write(value.c_str(), value.length() * sizeof(char), m_CurrentBufferOffset);
		m_CurrentBufferOffset += value.length() * sizeof(char);

		return sizeof(uint64_t) + (value.length() * sizeof(char));
	}

	template<typename T>
	size_t Write(T* data, size_t dataLength)
	{
		m_DataBuffer.Write((const void*)data, dataLength, m_CurrentBufferOffset);
		m_CurrentBufferOffset += dataLength;
		return dataLength;
	}

	bool Available() const { return m_CurrentBufferOffset < m_DataBuffer.Size; }
	void* GetData() const { return m_DataBuffer.Data; }
	size_t GetSize() { return m_DataBuffer.Size; }
	size_t GetCursor() { return m_CurrentBufferOffset; }
	void AddBufferOffset(size_t offset) { m_CurrentBufferOffset = offset; }
	void ResetCursor() { m_CurrentBufferOffset = 0; }

	void Resize(size_t size)
	{
		Buffer oldBuffer = m_DataBuffer;

		m_DataBuffer = Buffer();
		m_DataBuffer.Allocate(size);
		m_DataBuffer.TryWrite(oldBuffer.Data, oldBuffer.Size);
		oldBuffer.Release();
	}

private:
	Buffer m_DataBuffer;
	size_t m_CurrentBufferOffset = 0;
	uint32_t m_RefCount = 0;
	bool m_OwnsData = true;
};

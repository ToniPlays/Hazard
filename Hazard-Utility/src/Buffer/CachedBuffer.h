#pragma once

#include "Buffer.h"
#include "UtilityCore.h"
#include "Profiling/MemoryDiagnostic.h"
#include "Ref.h"

class CachedBuffer : public RefCount
{
public:

	CachedBuffer() = default;

	CachedBuffer(uint64_t size)
	{
		m_DataBuffer.Allocate(size);
		m_DataBuffer.ZeroInitialize();
	}

	CachedBuffer(void* data, uint64_t size)
	{
		m_DataBuffer = Buffer(data, size);
		m_OwnsData = false;
	}

	CachedBuffer(Buffer buffer)
	{
		m_DataBuffer = buffer;
		m_OwnsData = true;
	}

	~CachedBuffer()
	{
		if (m_OwnsData)
			m_DataBuffer.Release();
	}

	void Allocate(uint64_t size)
	{
		m_DataBuffer.Allocate(size);
		m_DataBuffer.ZeroInitialize();
	}

	template<typename T>
	T Read()
	{
		if (m_DataBuffer.Size < m_CurrentBufferOffset + sizeof(T)) assert(false);

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
	T Read(uint64_t size)
	{
		if (m_DataBuffer.Size < m_CurrentBufferOffset + size) assert(false);

		T* start = (T*)m_DataBuffer.Data + m_CurrentBufferOffset;
		m_CurrentBufferOffset += size;
		return std::vector<T>(start, start + size);
	}

	template<>
	Buffer Read(uint64_t size)
	{
		if (m_DataBuffer.Size < m_CurrentBufferOffset + size) assert(false);

		Buffer value((uint8_t*)m_DataBuffer.Data + m_CurrentBufferOffset, size);
		m_CurrentBufferOffset += size;
		return value;
	}
	

	template<typename T>
	uint64_t Write(T value)
	{
		m_DataBuffer.Write(&value, sizeof(T), m_CurrentBufferOffset);
		m_CurrentBufferOffset += sizeof(T);
		return sizeof(T);
	}

	template<>
	uint64_t Write(Buffer value)
	{
		m_DataBuffer.Write(value.Data, value.Size, m_CurrentBufferOffset);
		m_CurrentBufferOffset += value.Size;
		return value.Size;
	}

	template<>
	uint64_t Write(std::string value)
	{
		uint64_t length = value.length();
		m_DataBuffer.Write(&length, sizeof(uint64_t), m_CurrentBufferOffset);
		m_CurrentBufferOffset += sizeof(uint64_t);

		m_DataBuffer.Write(value.c_str(), length * sizeof(char), m_CurrentBufferOffset);
		m_CurrentBufferOffset += length * sizeof(char);

		return sizeof(uint64_t) + (length * sizeof(char));
	}

	template<typename T>
	uint64_t Write(T* data, uint64_t dataLength)
	{
		m_DataBuffer.Write((const void*)data, dataLength, m_CurrentBufferOffset);
		m_CurrentBufferOffset += dataLength;
		return dataLength;
	}

	bool Available() const { return m_CurrentBufferOffset < m_DataBuffer.Size; }
	void* GetData() const { return m_DataBuffer.Data; }
	uint64_t GetSize() const { return m_DataBuffer.Size; }
	uint64_t GetCursor() const { return m_CurrentBufferOffset; }
	void AddBufferOffset(uint64_t offset) { m_CurrentBufferOffset = offset; }
	void ResetCursor() { m_CurrentBufferOffset = 0; }

	void Resize(uint64_t size)
	{
		Buffer oldBuffer = m_DataBuffer;

		m_DataBuffer = Buffer();
		m_DataBuffer.Allocate(size);
		m_DataBuffer.TryWrite(oldBuffer.Data, oldBuffer.Size);
		oldBuffer.Release();
	}

private:
	Buffer m_DataBuffer;
	uint64_t m_CurrentBufferOffset = 0;
	bool m_OwnsData = false;
};

#pragma once

#include "UtilityCore.h"

template<typename Type>

class Batch 
{
public:
	Batch() = default;
	Batch(size_t size)
	{
		Resize(size);
	};
	~Batch()
	{
		m_DataBuffer.Release();
	};

	void Push(const Type& data)
	{
		m_DataBuffer.Write(&data, sizeof(Type), m_WriteBufferOffset);
		m_WriteBufferOffset += sizeof(Type);
	};
	inline void AddIndices(size_t count) { m_IndexCount += count; }
	void Reset() 
	{
		m_WriteBufferOffset = 0;
		m_IndexCount = 0;
	};
	void Resize(size_t size)
	{
		m_DataBuffer.Release();
		m_DataBuffer.Allocate(size * sizeof(Type));
		m_DataBuffer.ZeroInitialize();
		Reset();
	}

	const void* GetData() const { return m_DataBuffer.Data; }
	size_t GetCount() { return m_WriteBufferOffset / sizeof(Type); }
	size_t GetSize() { return m_DataBuffer.Size; }
	size_t GetDataSize() { return m_WriteBufferOffset; }
	size_t GetIndexCount() { return m_IndexCount; }
	operator bool() const { return m_IndexCount != 0; }
private:
	size_t m_IndexCount = 0;
	size_t m_WriteBufferOffset = 0;
	Buffer m_DataBuffer;
};

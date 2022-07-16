#pragma once

#include "UtilityCore.h"

template<typename Type>
class Batch {
public:
	Batch() : m_DataSize(sizeof(Type)) {}
	Batch(uint32_t size) : m_DataSize(sizeof(Type)) {
		Resize(size);
	};
	~Batch() {
	};

	void Push(const Type& data)
	{
		memcpy(m_BufferPointer, &data, m_DataSize);
		m_Count++;
		m_BufferPointer++;
	};
	inline void AddIndices(uint32_t count) { m_IndexCount += count; }
	void Reset() {
		m_BufferPointer = m_BufferBase;
		m_IndexCount = 0;
		m_Count = 0;
	};
	void Resize(uint32_t size)
	{
		m_Size = size;
		m_BufferSize = m_Size * m_DataSize;

		m_BufferBase = new Type[m_Size];
		Reset();
	}

	void* GetData() { return m_BufferBase; }
	uint32_t GetCount() { return m_Count; }
	uint32_t GetSize() { return m_Size; }
	uint32_t GetDataSize() { return (uint32_t)((uint8_t*)m_BufferPointer - (uint8_t*)m_BufferBase); }
	uint32_t GetIndexCount() { return m_IndexCount; }
	operator bool() const { return m_IndexCount != 0; }

private:
	uint32_t m_DataSize = 0;
	uint32_t m_Size = 0;
	uint32_t m_BufferSize = 0;
	uint32_t m_IndexCount = 0;
	uint32_t m_Count = 0;

	Type* m_BufferBase;
	Type* m_BufferPointer;
};

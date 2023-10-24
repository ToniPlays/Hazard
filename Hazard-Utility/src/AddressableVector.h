#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

template<typename K, typename V>
class AddressableVector
{
public:
	AddressableVector() = default;
	AddressableVector(uint64_t size)
	{
		m_Keys.reserve(size);
		m_Values.reserve(size);
	};

	~AddressableVector() = default;

	uint64_t Push(K key, V value)
	{
		m_Values.push_back(value);
		uint64_t index = m_Values.size() - 1;
		m_Keys[key] = index;
		return index;
	}
	bool Contains(K value)
	{
		return m_Keys.find(&value) != m_Keys.end();
	}
	uint64_t GetIndex(K value)
	{
		return m_Keys[value];
	}
	V& Get(uint64_t index)
	{
		return m_Values[index];
	}
	void* Data()
	{
		return m_Values.data();
	}
	uint64_t Size()
	{
		return m_Values.size();
	}
private:

	std::vector<V> m_Values;
	std::unordered_map<K, uint64_t> m_Keys;
};
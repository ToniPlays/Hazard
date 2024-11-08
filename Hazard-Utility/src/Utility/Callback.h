#pragma once

#include <vector>
#include <functional>

template<typename FuncT>
class Callback
{
public:

	Callback() = default;
	~Callback() = default;

	uint32_t Count() const { return m_Callbacks.size(); }

	inline void Add(const std::function<FuncT>& callback)
	{
		if (!callback) return;

		std::scoped_lock lock(m_Mutex);
		m_Callbacks.push_back(callback);
	}
	inline void Clear()
	{
		std::scoped_lock lock(m_Mutex);
		m_Callbacks.clear();
	}

	template<typename... Args>
	void Invoke(const Args... args)
	{
		std::scoped_lock lock(m_Mutex);
		if constexpr (!std::is_same<FuncT, void()>::value)
		{
			for (uint32_t i = 0; i < m_Callbacks.size(); i++)
				m_Callbacks[i](args...);
		}
		else assert(false);
	}

	void Invoke()
	{
		std::scoped_lock lock(m_Mutex);
		if constexpr (std::is_same<FuncT, void()>::value)
		{
			for(uint32_t i = 0; i < m_Callbacks.size(); i++)
				m_Callbacks[i]();
		}
		else assert(false);
	}

private:
	std::mutex m_Mutex;
	std::vector<std::function<FuncT>> m_Callbacks;
};
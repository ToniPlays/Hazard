#pragma once

#include <vector>
#include <functional>

template<typename FuncT>
class Callback
{
public:

	Callback() = default;
	~Callback()
	{
		Clear();
	}

	uint32_t Count() const { return m_Callbacks.size(); }

	void Add(const std::function<FuncT>& callback)
	{
		if (!callback) return;
		m_Callbacks.push_back(callback);
	}
	void Clear()
	{
		m_Callbacks.clear();
	}

	template<typename... Args>
	void Invoke(const Args... args)
	{
		if constexpr (!std::is_same<FuncT, void()>::value)
		{
			for (auto& cb : m_Callbacks)
				if (cb)
					cb(args...);
		}
		else assert(false);
	}

	void Invoke()
	{
		if constexpr (std::is_same<FuncT, void()>::value)
		{
			for (auto& cb : m_Callbacks)
				if (cb)
					cb();
		}
		else assert(false);
	}

private:
	std::vector<std::function<FuncT>> m_Callbacks;
};
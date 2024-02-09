#pragma once

#include "Callback.h"
#include <unordered_map>
#include <vector>

template<typename HookT, typename FuncT>
class Hooks
{
public:
	void AddHook(HookT hook, const std::function<FuncT>& callback)
	{
		m_Hooks[hook].Add(callback);
	}
	void Invoke(HookT hook)
	{
		m_Hooks[hook].Invoke();
	}
	template<typename... Args>
	void Invoke(HookT hook, const Args... args)
	{
		m_Hooks[hook].Invoke<Args...>(args...);
	}

private:
	std::unordered_map<HookT, Callback<FuncT>> m_Hooks;
};
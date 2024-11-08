#pragma once

#include <condition_variable>
#include <coroutine>
#include <functional>

class Coroutine
{
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

public:
	Coroutine() = default;
	Coroutine(handle_type handle) : m_Handle(handle) {}
	~Coroutine() = default;

	void MoveNext()
	{
		m_Handle.resume();
	}

	bool CanContinue() const 
	{
		if (!m_Handle) return true;
		return m_Handle.promise().m_Dependencies == 0;
	}

	bool Done() const
	{
		return m_Handle.done();
	}

	struct promise_type
	{
		handle_type handle()
		{
			return handle_type::from_promise(*this);
		}
		auto initial_suspend()
		{
			return std::suspend_always();
		}
		auto final_suspend() noexcept
		{
			return std::suspend_always();
		}
		auto get_return_object() noexcept
		{
			return Coroutine(handle());
		}

		auto return_void() {}
		void unhandled_exception() {}

		std::atomic_uint32_t m_Dependencies = 0;
	};

	operator bool() { return m_Handle.address(); }

private:
	handle_type m_Handle;
};
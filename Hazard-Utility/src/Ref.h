#pragma once

#include <iostream>
#include <assert.h>

#include "UtilityCore.h"

class RefCount 
{
public:
	RefCount() { m_RefCount = 0; };
	virtual ~RefCount() = default;

	void IncRefCount() const 
	{
		m_RefCount++;
	}
	void DecRefCount() const 
	{
		m_RefCount--;
	}
	uint32_t GetRefCount() const 
	{ 
		return m_RefCount.load();
	}

private:
	mutable std::atomic_uint32_t m_RefCount;
};


template<typename T>
class Ref
{
public:
	Ref()
		: m_Instance(nullptr) {}

	Ref(std::nullptr_t n)
		: m_Instance(nullptr) {}

	Ref(T* instance)
		: m_Instance(instance)
	{
		static_assert(std::is_base_of<RefCount, T>::value, "Class is not RefCounted!");
		IncRef();
	}

	template<typename T2>
	Ref(const Ref<T2>& other)
	{
		m_Instance = (T*)other.m_Instance;
		IncRef();
	}

	template<typename T2>
	Ref(Ref<T2>&& other)
	{
		m_Instance = (T*)other.m_Instance;
		other.m_Instance = nullptr;
	}

	~Ref()
	{
		DecRef();
	}

	Ref(const Ref<T>& other)
		: m_Instance(other.m_Instance)
	{
		IncRef();
	}

	Ref& operator=(std::nullptr_t)
	{
		DecRef();
		m_Instance = nullptr;
		return *this;
	}

	Ref& operator=(const Ref<T>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;
		return *this;
	}

	template<typename T2>
	Ref& operator=(const Ref<T2>& other)
	{
		other.IncRef();
		DecRef();

		m_Instance = other.m_Instance;
		return *this;
	}

	template<typename T2>
	Ref& operator=(Ref<T2>&& other)
	{
		DecRef();

		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;

		return *this;
	}
	template<typename T2>
	[[nodiscard]] Ref<T2> As() const
	{
		return Ref<T2>(*this);
	}

	operator bool() { return m_Instance != nullptr; }
	operator bool() const { return m_Instance != nullptr; }

	T* operator->() { return m_Instance; }
	const T* operator->() const { return m_Instance; }

	T& operator*() { return *m_Instance; }
	const T& operator*() const { return *m_Instance; }

	bool operator==(const Ref<T>& other) const
	{
		return m_Instance == other.m_Instance;
	}

	T* Raw() { return  m_Instance; }
	const T* Raw() const { return  m_Instance; }

	void Release(T* instance = nullptr)
	{
		DecRef();
		m_Instance = instance;
	}

	template<typename... Args>
	static Ref<T> Create(Args&&... args)
	{
		return Ref<T>(hnew T(std::forward<Args>(args)...));
	}
private:
	void IncRef() const
	{
		if (m_Instance != nullptr)
			m_Instance->IncRefCount();
	}

	void DecRef() const
	{
		if (m_Instance != nullptr)
		{
			m_Instance->DecRefCount();
			if (m_Instance->GetRefCount() == 0)
				hdelete m_Instance;
		}
	}

	template<class T2>
	friend class Ref;
	T* m_Instance = nullptr;
};

template<class T, class RT = void>
struct IsRef 
{
	static constexpr bool value = false;
};

template<class RT>
struct IsRef<Ref<RT>> 
{
	static constexpr bool value = true;
};

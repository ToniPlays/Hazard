#pragma once

#include <map>
#include <mutex>

#if defined(HZR_DEBUG) && defined(HZR_PLATFORM_WINDOWS)
#define HZR_MEM_DIAG
#endif

#define KB	* 1024
#define MB	* 131072
#define GB	* 1073741824


namespace Memory
{
	struct MemoryStats
	{
		size_t TotalAllocated = 0;
		size_t TotalFreed = 0;
	};

	struct MemoryAllocation
	{
		void* Memory = nullptr;
		size_t Size = 0;
		const char* Category = nullptr;
	};

	const MemoryStats& GetAllocationStats();

	template <class T>
	struct Mallocator
	{
		typedef T value_type;

		Mallocator() = default;
		template<class U>
		constexpr Mallocator(const Mallocator<U>&) noexcept {}

		T* allocate(std::size_t size)
		{
#undef max
			if (size > std::numeric_limits<std::size_t>::max() / sizeof(T))
				throw std::bad_array_new_length();

			if (auto p = static_cast<T*>(std::malloc(size * sizeof(T))))
				return p;

			throw std::bad_alloc();
		}
		void deallocate(T* p, std::size_t size) noexcept
		{
			free(p);
		}
	};

	struct AllocatorData
	{
		using MapAlloc = Mallocator<std::pair<const void* const, MemoryAllocation>>;
		using StatsMapAlloc = Mallocator<std::pair<const char* const, MemoryStats>>;

		using AllocationStatsMap = std::map<const char*, MemoryStats, std::less<const char*>, StatsMapAlloc>;

		std::map<const void*, MemoryAllocation, std::less<const void*>, MapAlloc> m_AllocationMap;
		AllocationStatsMap m_AllocationStatsMap;

		std::mutex m_Mutex, m_StatsMutex;
	};

	class Allocator
	{
	public:
		static void Init();

		static void* AllocateRaw(size_t size);
		static void* Allocate(size_t size);
		static void* Allocate(size_t size, const char* desc);
		static void* Allocate(size_t size, const char* file, int line);
		static void Free(void* memory);

		static const AllocatorData::AllocationStatsMap& GetAllocationStats()
		{
			return s_Data->m_AllocationStatsMap;
		}
	private:
		inline static AllocatorData* s_Data = nullptr;
	};
}


#ifdef HZR_MEM_DIAG
    #ifdef HZR_PLATFORM_WINDOWS
        #define _MEM_FUNC_DEC_ _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
        #define _FUNC_DECL_ __CRTDECL
    #elif HZR_PLATFORM_MACOS
        #define _MEM_FUNC_DECL_
        #define _FUNC_DECL_
    #endif

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new(size_t size);

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new[](size_t size);

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new(size_t size, const char* desc);

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new[](size_t size, const char* desc);

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new(size_t size, const char* file, int line);

    _MEM_FUNC_DECL_
    void* _FUNC_DECL_ operator new[](size_t size, const char* file, int line);

    void _FUNC_DECL_ operator delete(void* memory);
    void _FUNC_DECL_ operator delete(void* memory, const char* desc);
    void _FUNC_DECL_ operator delete(void* memory, const char* file, int line);

    void _FUNC_DECL_ operator delete[](void* memory);
    void _FUNC_DECL_ operator delete[](void* memory, const char* desc);
    void _FUNC_DECL_ operator delete[](void* memory, const char* file, int line);

    #define hnew new(__FILE__, __LINE__)
    #define hdelete delete
#else

    #define hnew new
    #define hdelete delete
#endif


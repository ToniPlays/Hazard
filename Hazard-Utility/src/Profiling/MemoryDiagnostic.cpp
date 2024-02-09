#include "MemoryDiagnostic.h"

#include <iostream>
#include <memory>
#include <mutex>
#include <Profiling/PerformanceProfiler.h>
#include <cassert>

namespace Memory
{
	static MemoryStats s_GlobalStats;

	static bool s_InInit = false;

	const MemoryStats& GetAllocationStats()
	{
		return s_GlobalStats;
	}

	void Allocator::Init()
	{
		if (s_Data)
			return;

		s_InInit = true;

		AllocatorData* data = (AllocatorData*)Allocator::AllocateRaw(sizeof(AllocatorData));
		new(data) AllocatorData();
		s_Data = data;
		s_InInit = false;
	}
	void* Allocator::AllocateRaw(size_t size)
	{
		return malloc(size);
	}
	void* Allocator::Allocate(size_t size)
	{
		if (s_InInit) 
			return AllocateRaw(size);

		if (!s_Data)
			Init();

		void* memory = malloc(size);

		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			MemoryAllocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;

			s_GlobalStats.TotalAllocated += size;
		}
		return memory;
	}
	void* Allocator::Allocate(size_t size, const char* desc)
	{
		if (!s_Data)
			Init();

		void* memory = malloc(size);
		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			MemoryAllocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;
			alloc.Category = desc;

			s_GlobalStats.TotalAllocated += size;

			if (desc)
				s_Data->m_AllocationStatsMap[desc].TotalAllocated += size;
		}
		return memory;
	}
	void* Allocator::Allocate(size_t size, const char* file, int line)
	{
		if (!s_Data)
			Init();

		void* memory = malloc(size);
		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			MemoryAllocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;
			alloc.Category = file;

			s_GlobalStats.TotalAllocated += size;
			s_Data->m_AllocationStatsMap[file].TotalAllocated += size;
		}
		return memory;
	}
	void Allocator::Free(void* memory)
	{
		if (memory == nullptr)
			return;
		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			if (s_Data->m_AllocationMap.find(memory) != s_Data->m_AllocationMap.end())
			{
				const MemoryAllocation& alloc = s_Data->m_AllocationMap.at(memory);
				s_GlobalStats.TotalFreed += alloc.Size;

				if (alloc.Category)
					s_Data->m_AllocationStatsMap[alloc.Category].TotalFreed += alloc.Size;

				s_Data->m_AllocationMap.erase(memory);
			}
			else
			{
				std::cout << "Allocation for " << memory << " not found" << std::endl;
				assert(false);
			}
		}
        free(memory);
	}
}

#ifdef HZR_MEM_DIAG
    #ifdef HZR_PLATFORM_WINDOWS

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new(size_t size)
    {
        return Memory::Allocator::Allocate(size);
    }

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new[](size_t size)
    {
        return Memory::Allocator::Allocate(size);
    }

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new(size_t size, const char* desc)
    {
        return Memory::Allocator::Allocate(size, desc);
    }

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new[](size_t size, const char* desc)
    {
        return Memory::Allocator::Allocate(size, desc);
    }

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new(size_t size, const char* file, int line)
    {
        return Memory::Allocator::Allocate(size, file, line);
    }

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __CRTDECL operator new[](size_t size, const char* file, int line)
    {
        return Memory::Allocator::Allocate(size, file, line);
    }

    void __CRTDECL operator delete(void* memory)
    {
        return Memory::Allocator::Free(memory);
    }
    void __CRTDECL operator delete(void* memory, const char* desc)
    {
        return Memory::Allocator::Free(memory);
    }
    void __CRTDECL operator delete(void* memory, const char* file, int line)
    {
        return Memory::Allocator::Free(memory);
    }

    void __CRTDECL operator delete[](void* memory)
    {
        return Memory::Allocator::Free(memory);
    }
    void __CRTDECL operator delete[](void* memory, const char* desc)
    {
        return Memory::Allocator::Free(memory);
    }
    void __CRTDECL operator delete[](void* memory, const char* file, int line)
    {
        return Memory::Allocator::Free(memory);
    }
    #endif
#endif

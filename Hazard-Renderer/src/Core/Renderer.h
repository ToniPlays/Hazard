#pragma once

#include "CommandQueue.h"
#include "GraphicsContext.h"
#include "Profiling/MemoryDiagnostic.h"

#define FREE_QUEUE_COUNT 3

namespace HazardRenderer
{
	struct CommandQueues
	{
		CommandQueue* RenderCommandQueue;
		CommandQueue* ResourceCreateCommandQueue;
		CommandQueue* ResourceFreeCommandQueue[FREE_QUEUE_COUNT];

		//			 F		 R
		//---1--- ---2--- ---3--- ---4--- ---1--- ---2--- ---3--- ---4---
		//					-->
	};

	class Renderer
	{
	public:
		static void Init(GraphicsContext* context)
		{
			s_GraphicsContext = context;

			s_CommandQueue.ResourceCreateCommandQueue = hnew CommandQueue(1 MB);
			s_CommandQueue.RenderCommandQueue = hnew CommandQueue(10 MB);

			for (uint32_t i = 0; i < FREE_QUEUE_COUNT; i++)
				s_CommandQueue.ResourceFreeCommandQueue[i] = hnew CommandQueue(1 MB);
		}

		static uint64_t GetRenderedFrameCount() { return s_RenderedFrames; }

		static void WaitAndRender()
		{
			HZR_PROFILE_FUNCTION();
			auto& queue = s_CommandQueue;
			{
				HZR_PROFILE_FUNCTION("ResourceCreateQueue::Execute()");
				queue.ResourceCreateCommandQueue->Excecute();
				queue.ResourceCreateCommandQueue->Clear();
			}
			{
				HZR_PROFILE_FUNCTION("RenderCommandQueue::Execute()");
				s_IsRendering = true;
				s_IsRendering.notify_all();

				queue.RenderCommandQueue->Excecute();
				queue.RenderCommandQueue->Clear();

				s_RenderedFrames++;
				s_IsRendering = false;
				s_IsRendering.notify_all();
			}
			{
				HZR_PROFILE_FUNCTION("ResourceFreeQueue::Execute()");
				queue.ResourceFreeCommandQueue[s_RenderedFrames % FREE_QUEUE_COUNT]->Excecute();
				queue.ResourceFreeCommandQueue[s_RenderedFrames % FREE_QUEUE_COUNT]->Clear();
			}
		}

		template<typename FuncT>
		static void Submit(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};

			s_IsRendering.wait(true);

			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto& queue = s_CommandQueue;
			auto storageBuffer = queue.RenderCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};

			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto& queue = s_CommandQueue;
			auto storageBuffer = queue.ResourceCreateCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};

			auto& queue = s_CommandQueue;
			auto storageBuffer = queue.ResourceFreeCommandQueue[s_RenderedFrames % FREE_QUEUE_COUNT]->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		template<typename FuncT>
		static void SubmitResourceDestroy(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};

			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto& queue = s_CommandQueue;
			auto storageBuffer = queue.ResourceFreeCommandQueue[s_RenderedFrames % FREE_QUEUE_COUNT]->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

	private:
		static inline GraphicsContext* s_GraphicsContext = nullptr;
		static inline CommandQueues s_CommandQueue;
		static inline std::atomic_bool s_IsRendering = false;
		static inline std::mutex s_ResourceMutex;
		static inline uint64_t s_RenderedFrames = 0;
	};
}

#pragma once

#include "CommandQueue.h"
#include "GraphicsContext.h"
#include "Profiling/MemoryDiagnostic.h"

namespace HazardRenderer
{
	struct CommandQueues
	{
		CommandQueue* RenderCommandQueue;
		CommandQueue* ResourceCreateCommandQueue;
		CommandQueue* ResourceFreeCommandQueue;

	};

	class Renderer
	{
	public:
		static void Init(GraphicsContext* context)
		{
			s_GraphicsContext = context;

				s_CommandQueue.RenderCommandQueue = hnew CommandQueue(0.5f MB);
				s_CommandQueue.ResourceCreateCommandQueue = hnew CommandQueue(2.0f MB);
				s_CommandQueue.ResourceFreeCommandQueue = hnew CommandQueue(0.5f MB);
		}

		static void WaitAndRender()
		{
			HZR_PROFILE_FUNCTION();
			auto& queue = s_CommandQueue;
			s_IsRendering = true;
			{
				HZR_PROFILE_FUNCTION("ResourceCreateQueue::Execute()");
				queue.ResourceCreateCommandQueue->Excecute();
				queue.ResourceCreateCommandQueue->Clear();
			}
			{
				HZR_PROFILE_FUNCTION("RenderCommandQueue::Execute()");
				queue.RenderCommandQueue->Excecute();
				queue.RenderCommandQueue->Clear();
			}
			{
				HZR_PROFILE_FUNCTION("ResourceFreeQueue::Execute()");
				queue.ResourceFreeCommandQueue->Excecute();
				queue.ResourceFreeCommandQueue->Clear();
			}
			s_IsRendering = false;
			s_IsRendering.notify_all();
		}

		template<typename FuncT>
		static void Submit(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				//pFunc->~FuncT();
			};

			s_IsRendering.wait(true);
			HZR_ASSERT(!s_IsRendering, "Cannot submit while rendering");
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
			
			s_IsRendering.wait(true);
			HZR_ASSERT(!s_IsRendering, "Cannot submit while rendering");
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

			s_IsRendering.wait(true);
			HZR_ASSERT(!s_IsRendering, "Cannot submit while rendering");
			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto& queue = s_CommandQueue;
			auto storageBuffer = queue.ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
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
			auto storageBuffer = queue.ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

	private:
		static inline GraphicsContext* s_GraphicsContext = nullptr;
		static inline CommandQueues s_CommandQueue;
		static inline std::mutex s_ResourceMutex;
		static inline std::atomic_bool s_IsRendering;
	};
}

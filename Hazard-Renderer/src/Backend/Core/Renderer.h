#pragma once

#include "CommandQueue.h"
#include "GraphicsContext.h"

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
			uint32_t framesInFlight = 3;

			s_CommandQueue.RenderCommandQueue = hnew CommandQueue(5 MB);
			s_CommandQueue.ResourceCreateCommandQueue = hnew CommandQueue(20 MB);
			s_CommandQueue.ResourceFreeCommandQueue = hnew CommandQueue(5 MB);

			s_GraphicsContext = context;
		}
		static void WaitAndRender()
		{
			//s_IsExecuting = true;
			HZR_PROFILE_FUNCTION();
			{
				HZR_PROFILE_FUNCTION("ResourceCreateQueue::Execute()");
				s_CommandQueue.ResourceCreateCommandQueue->Excecute();
				s_CommandQueue.ResourceCreateCommandQueue->Clear();
			}
			{
				HZR_PROFILE_FUNCTION("RenderCommandQueue::Execute()");
				s_CommandQueue.RenderCommandQueue->Excecute();
				s_CommandQueue.RenderCommandQueue->Clear();
			}
			{
				HZR_PROFILE_FUNCTION("ResourceFreeQueue::Execute()");
				s_CommandQueue.ResourceFreeCommandQueue->Excecute();
				s_CommandQueue.ResourceFreeCommandQueue->Clear();
			}
			//s_IsExecuting = false;
			//s_IsExecuting.notify_all();
		}

		template<typename FuncT>
		static void Submit(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};

			//s_IsExecuting.wait(true);
			//std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto storageBuffer = s_CommandQueue.RenderCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				//pFunc->~FuncT();
			};

			//s_IsExecuting.wait(true);
			//std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto storageBuffer = s_CommandQueue.ResourceCreateCommandQueue->Allocate(renderCmd, sizeof(func));
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
			//s_IsExecuting.wait(true);
			//std::scoped_lock<std::mutex> lock{ s_ResourceMutex };

			auto storageBuffer = s_CommandQueue.ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
	private:
		static inline GraphicsContext* s_GraphicsContext = nullptr;
		static inline CommandQueues s_CommandQueue;
		static inline std::mutex s_ResourceMutex;
		static inline std::atomic_bool s_IsExecuting = false;
	};
}
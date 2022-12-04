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
			s_CommandQueues.resize(framesInFlight);

			for (auto& queue : s_CommandQueues)
			{
				queue.RenderCommandQueue = hnew CommandQueue(5 MB);
				queue.ResourceCreateCommandQueue = hnew CommandQueue(20 MB);
				queue.ResourceFreeCommandQueue = hnew CommandQueue(5 MB);
			}

			s_CurrentQueue = 0;
			m_GraphicsContext = context;
		}
		static void WaitAndRender()
		{
			HZR_PROFILE_FUNCTION();
			auto& queue = s_CommandQueues[s_CurrentQueue];
			s_CurrentQueue = (s_CurrentQueue + 1) % s_CommandQueues.size();
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
		}

		template<typename FuncT>
		static void Submit(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto storageBuffer = s_CommandQueues[s_CurrentQueue].RenderCommandQueue->Allocate(renderCmd, sizeof(func));
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
			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto storageBuffer = s_CommandQueues[s_CurrentQueue].ResourceCreateCommandQueue->Allocate(renderCmd, sizeof(func));
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
			std::scoped_lock<std::mutex> lock{ s_ResourceMutex };
			auto storageBuffer = s_CommandQueues[s_CurrentQueue].ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
	private:
		static inline GraphicsContext* m_GraphicsContext = nullptr;
		static inline std::vector<CommandQueues> s_CommandQueues;
		static inline uint32_t s_CurrentQueue = 0;
		static inline std::mutex s_ResourceMutex;
	};
}
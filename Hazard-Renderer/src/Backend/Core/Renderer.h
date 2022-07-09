#pragma once

#include "CommandQueue.h"

namespace HazardRenderer {
	class Renderer {
	public:

		static void Init() {
			s_RenderCommandQueue = new CommandQueue();
			s_ResourceCreateCommandQueue = new CommandQueue();
			s_ResourceFreeCommandQueue = new CommandQueue();
		}

		static void WaitAndRender() 
		{
			HZR_PROFILE_SCOPE("ResourceCreateQueue::Execute()");
			s_ResourceCreateCommandQueue->Excecute();

			HZR_PROFILE_SCOPE("RenderCommandQueue::Execute()");
			s_RenderCommandQueue->Excecute();

			HZR_PROFILE_SCOPE("ResourceFreeQueue::Execute()");
			s_ResourceFreeCommandQueue->Excecute();

			s_ResourceCreateCommandQueue->Clear();
			s_RenderCommandQueue->Clear();
			s_ResourceFreeCommandQueue->Clear();
		}

		template<typename FuncT>
		static void Submit(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_RenderCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_ResourceCreateCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
	private:

		static inline CommandQueue* s_RenderCommandQueue;
		static inline CommandQueue* s_ResourceCreateCommandQueue;
		static inline CommandQueue* s_ResourceFreeCommandQueue;
	};
}
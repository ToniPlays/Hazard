#pragma once

#include "CommandQueue.h"
#include "GraphicsContext.h"

#define HZR_RENDER_THREAD_ONLY() { assert(!Renderer::s_CanSubmit); }

namespace HazardRenderer 
{
	class Renderer 
	{
	public:

		static void Init(GraphicsContext* context)
		{
			s_RenderCommandQueue = hnew CommandQueue(5 MB);
			s_ResourceCreateCommandQueue = hnew CommandQueue(20 MB);
			s_ResourceFreeCommandQueue = hnew CommandQueue(5 MB);

			m_GraphicsContext = context;
		}

		static void BeginFrame() 
		{
			//m_GraphicsContext->PrepareFrame();
		}

		static void WaitAndRender()
		{
			HZR_PROFILE_FUNCTION();
			s_CanSubmit = false;
			{
				HZR_PROFILE_FUNCTION("ResourceCreateQueue::Execute()");
				s_ResourceCreateCommandQueue->Excecute();
			}
			{
				HZR_PROFILE_FUNCTION("RenderCommandQueue::Execute()");
 				s_RenderCommandQueue->Excecute();
			}
			{
				HZR_PROFILE_FUNCTION("ResourceFreeQueue::Execute()");
				s_ResourceFreeCommandQueue->Excecute();
			}
			s_ResourceCreateCommandQueue->Clear();
			s_RenderCommandQueue->Clear();
			s_ResourceFreeCommandQueue->Clear();
			s_CanSubmit = true;
		}

		template<typename FuncT>
		static void Submit(FuncT func) 
		{
			HZR_ASSERT(s_CanSubmit, "Cannot submit while rendering");
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_RenderCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT func) 
		{
			HZR_ASSERT(s_CanSubmit, "Cannot submit create while rendering");
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_ResourceCreateCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT func) 
		{
			HZR_ASSERT(s_CanSubmit, "Cannot submit free while rendering");
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_ResourceFreeCommandQueue->Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
	private:
		static inline GraphicsContext* m_GraphicsContext = nullptr;
		static inline CommandQueue* s_RenderCommandQueue;
		static inline CommandQueue* s_ResourceCreateCommandQueue;
		static inline CommandQueue* s_ResourceFreeCommandQueue;

	public:
		static inline bool s_CanSubmit = true;
	};
}
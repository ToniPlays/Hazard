#pragma once

#include "RenderContext.h"
#include "Pipeline/Buffers.h"
#include "Pipeline/Pipeline.h"

namespace Hazard::Rendering {

	class RenderContextCommand {
		friend class RenderContext;
	public:
		static void SetTitle(const char* title);

		static float GetWindowWidth();
		static float GetWindowHeight();
		static bool IsFullscreen();
		static void SetFullscreen(bool fullscreen);

		static bool IsVsync();
		static void SetVsync(bool vsync);

		template<typename FuncT>
		static void Submit(FuncT func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};

			auto storageBuffer = m_Context->GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};

			auto storageBuffer = m_Context->GetResourceCreateCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};

			auto storageBuffer = m_Context->GetResourceFreeCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		
		static void BeginRenderPass(Ref<RenderCommandBuffer> cmdBuffer, Ref<RenderPass> renderPass);
		static void EndRenderPass(Ref<RenderCommandBuffer> cmdBuffer);
		static void SetLineWidth(Ref<RenderCommandBuffer> cmdBuffer, float lineWidth);
		static void DrawGeometry(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline);
		static void DrawGeometry(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, uint32_t count);
		static void DrawGeometry_RT(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline);
		static void DrawGeometry_RT(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, uint32_t count);

		static void DrawGeometryArray(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<Pipeline> pipeline, uint32_t count);
		static void DispatchPipeline(Ref<RenderCommandBuffer> cmdBuffer, Ref<Pipeline> pipeline, uint32_t count);

		static RenderContext& GetContext() { return *m_Context; };
		static void SetDebugCallback(ErrorCallback& callback);
		static void SetClearColor(const Color& color);
		static uint32_t GetImagesInFlight() { return m_Context->GetImagesInFlight(); }
		static RenderAPI GetRenderAPI() { return m_Context->GetCurrentAPI(); }

	private:
		static void Init(RenderContext* context) { m_Context = context; }
		static RenderContext* m_Context;
	};
}
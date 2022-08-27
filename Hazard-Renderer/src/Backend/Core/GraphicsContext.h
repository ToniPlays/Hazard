#pragma once

#include "RenderContextCreateInfo.h"
#include "PhysicalDevice.h"
#include "RenderCommandBuffer.h"
#include "Pipeline/RenderPass.h"
#include "Swapchain.h"
#include <functional>

namespace HazardRenderer {

	using namespace Hazard;
	using ResizeCallback = std::function<void(uint32_t, uint32_t)>;

	enum class Topology { Points, Line, LineLoop, LineStrip, Triangle, TriangleStrip, TriangleFan, Outline };

	struct ErrorData {
		std::string Info;
		std::string ErrorCode;
		int Severity;

		ErrorData(const char* message, const char* type) : Info(message), ErrorCode(type), Severity(0) {}
	};

	using ErrorCallback = void(*)(ErrorData& data);

	class Window;
	struct WindowProps;

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init(Window* window, HazardRendererCreateInfo* info) = 0;
		virtual void BeginFrame() = 0;
		virtual void Present() = 0;

		virtual void BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, Ref<RenderPass> renderPass) = 0;
		virtual void EndRenderPass(Ref<RenderCommandBuffer> commandBuffer) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewport(int x, int y, int w, int h) = 0;

		virtual Ref<PhysicalDevice> GetDevice() = 0;
		virtual Ref<Swapchain> GetSwapchain() = 0;

		static GraphicsContext* Create(WindowProps* props);
		static RenderAPI GetRenderAPI() { return s_CurrentAPI; }

	private:
		static inline RenderAPI s_CurrentAPI;
	};
}
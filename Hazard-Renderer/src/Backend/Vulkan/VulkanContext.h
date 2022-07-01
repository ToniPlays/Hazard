#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/GraphicsContext.h"
#include "CommandQueue.h"
#include "Core/WindowSurface.h"
#include "Core/VulkanDevice.h"
#include "Core/VulkanSwapChain.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(result) if(result != VK_SUCCESS) { HZR_ASSERT(false, "We failed"); }

namespace HazardRenderer::Vulkan {

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;
		void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		void BeginFrame() override;
		void Present() override;

		void SetViewport(int x, int y, int w, int h) override;
		/*
		void AddResizeCallback(const ResizeCallback& callback) { m_SwapChain->AddResizeCallback(callback); };
		void SetErrorListener(const ErrorCallback& callback) override;
		PhysicalDevice& GetPhysicalDevice() const override;

		static VkInstance GetInstance() { return m_Instance; }
		static VkSurfaceKHR GetSurface() { return m_WindowSurface->GetVkSurface(); }
		static VulkanDevice* GetDevice() { return m_Device.get(); }
		static VkPipelineCache GetPipelineCache() { return m_Device->GetPipelineCache(); }
		inline static Ref<VulkanSwapChain> GetSwapchain() { return m_SwapChain; }

		static glm::vec4 GetClearColor() { return m_ClearColor; }
		static VkDescriptorSet RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo);
		*/

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
		void BeginRenderPass_RT(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass);
		void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
		void EndRenderPass_RT(Ref<RenderCommandBuffer> buffer);
		//void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth);
	
		
		PhysicalDevice& GetDevice() { return *m_Device; }
		Ref<Swapchain> GetSwapchain() override { return s_Instance->m_Swapchain.As<Swapchain>(); }


		static VulkanDevice& GetPhysicalDevice() { return (VulkanDevice&)s_Instance->GetDevice(); }
		static VkInstance GetVulkanInstance() { return s_Instance->m_Instance; }
		static Ref<VulkanSwapchain> GetVulkanSwapchain() { return s_Instance->m_Swapchain; }
		static WindowSurface& GetWindowSurface() { return *s_Instance->m_WindowSurface; }
		static glm::vec4& GetClearColor() { return s_Instance->m_ClearColor; }

		template<typename FuncT>
		static void Submit(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_Instance->m_RenderCommandQueue.Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceCreate(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_Instance->m_ResourceCreateCommandQueue.Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT func) {
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = s_Instance->m_ResourceFreeCommandQueue.Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

	private:

		inline static VulkanContext* s_Instance;

		Window* m_Window;
		glm::vec4 m_ClearColor = { 0, 0, 0, 1 };

		VkInstance m_Instance;
		Scope<WindowSurface> m_WindowSurface;
		Scope<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
	};
}
#endif

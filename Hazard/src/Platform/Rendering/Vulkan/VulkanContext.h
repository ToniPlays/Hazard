#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Core/VulkanDevice.h"
#include "Core/WindowSurface.h""
#include "Core/VulkanAllocator.h"
#include "Core/VulkanSwapChain.h"
#include "VulkanRenderCommandBuffer.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(result) if(result != VK_SUCCESS) { HZR_CORE_ERROR("Vulkan function failed: {0}", VKUtils::ResultToString(result));}

namespace Hazard::Rendering::Vulkan {

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void SwapBuffers() override;
		void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		void SetViewport(int x, int y, int w, int h) override;

		void AddResizeCallback(const ResizeCallback& callback) { m_SwapChain->AddResizeCallback(callback); };
		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

		static VkInstance GetInstance() { return m_Instance; }
		static VkSurfaceKHR GetSurface() { return m_WindowSurface->GetVkSurface(); }
		static VulkanDevice* GetDevice() { return m_Device.get(); }
		static VkPipelineCache GetPipelineCache() { return m_Device->GetPipelineCache(); }
		inline static Ref<VulkanSwapChain> GetSwapchain() { return m_SwapChain; }

		static glm::vec4 GetClearColor() { return m_ClearColor; }
		static VkDescriptorSet RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo);

		void BeginFrame();
		void Begin() override;
		void End();

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
		void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
		
	public:
		static void SendDebugMessage(const char* message, const char* code);
	private:

		Window* m_Window;
		inline static VkInstance m_Instance;
		inline static Scope<VulkanDevice> m_Device;
		inline static Scope<WindowSurface> m_WindowSurface;
		inline static Ref<VulkanSwapChain> m_SwapChain;
		inline static VkPipelineCache m_PipelineCache;
		inline static glm::vec4 m_ClearColor = { 0, 0, 0, 1 };

		inline static std::unordered_map<uint32_t, uint32_t> m_DescriptorAllocations;
		
		uint32_t m_CurrentBufferIndex = 0;
		static ErrorCallback s_Callback;
	};
}
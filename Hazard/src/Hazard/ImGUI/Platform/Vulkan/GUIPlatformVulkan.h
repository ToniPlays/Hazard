#pragma once

#include <hzrpch.h>
#ifdef HZR_INCLUDE_VULKAN

#include "../GUIPlatform.h"
#include "../ImGui_Backend/imgui_impl_vulkan.h"
#include "Vulkan/VulkanContext.h"

namespace Hazard
{
	class GUIPlatformVulkan : public GUIPlatform
	{
	public:
		GUIPlatformVulkan(HazardRenderer::Window& window);
		~GUIPlatformVulkan();

		void BeginFrame() override;
		void EndFrame() override;
		void Close() override;

	private:
		HazardRenderer::Window* m_Window;
		HazardRenderer::Vulkan::VulkanContext* m_Context = nullptr;
	};
}
#endif

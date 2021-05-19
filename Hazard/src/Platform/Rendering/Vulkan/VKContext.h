#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Core/VulkanInstance.h"
#include "Core/VulkanDevice.h"
#include "Core/VulkanValidationLayer.h"

namespace Hazard::Rendering::Vulkan {

	struct VulkanData 
	{
		GLFWwindow* window;
		VulkanInstance* vkInstance;
	};


	class VKContext : public GraphicsContext {

	public:
		VKContext(WindowProps* props);
		~VKContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void ClearFrame(glm::vec4 clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void SetDepthTest(DepthFunc type) const override;
		void Enable(Function fn) const;
		void Disable(Function fn) const;

		void DrawIndexed(VertexArray& array, uint32_t size) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

		VulkanData GetData() const { return m_VulkanData; }

	public:
		static void SendDebugMessage(const char* message, const char* code);
		static ErrorCallback s_Callback;

	private:
		VulkanData m_VulkanData;
	};
}
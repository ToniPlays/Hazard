#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include <GLFW/glfw3.h>
#include "Core/Instance.h"
#include "Device/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan {

	class VKContext : public GraphicsContext {

	public:
		VKContext(WindowProps* props);
		~VKContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void ClearFrame() const override;
		void SetClearColor(glm::vec4 clearColor) override;
		void SetViewport(int x, int y, int w, int h) const override;
		void Submit(CommandBuffer* buffer);
		//void SetDepthTest(DepthFunc type) const override;
		//void DrawIndexed(VertexArray& array, uint32_t size, DrawType type) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

		Instance* GetInstance() { return m_Instance; }
		VulkanDevice* GetDevice() { return m_Device; }

	public:
		static void SendDebugMessage(const char* message, const char* code);
		static ErrorCallback s_Callback;

	private:
		GLFWwindow* m_Window;
		Instance* m_Instance;
		VulkanDevice* m_Device;

		uint32_t m_ImagesInFlight;
	};
}
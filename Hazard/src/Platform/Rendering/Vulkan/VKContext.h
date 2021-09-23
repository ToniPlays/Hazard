#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
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


	public:
		static void SendDebugMessage(const char* message, const char* code);
		static Instance* GetInstance() { return m_Instance; }
		static VulkanDevice* GetDevice() { return m_Device; }

	private:
		GLFWwindow* m_Window = nullptr;
		uint32_t m_ImagesInFlight = 1;

		static ErrorCallback s_Callback;
		static Instance* m_Instance;
		static VulkanDevice* m_Device;

	};
}
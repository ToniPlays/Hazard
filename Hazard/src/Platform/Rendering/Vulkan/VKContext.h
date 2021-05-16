#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Core/VKInstance.h"
#include "Core/VKDevice.h"
#include "Core/VKValidationLayer.h"

namespace Hazard::Rendering::Vulkan {

	struct VulkanData 
	{
		GLFWwindow* window;
		VKInstance* vkInstance;
	};


	class VKContext : public GraphicsContext {

	public:
		VKContext(WindowProps* props);
		~VKContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void ClearFrame(glm::vec4 clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void SetDepthTest(DepthTest type) const override;
		void Enable(Function fn) const;
		void Disable(Function fn) const;

		void DrawIndexed(VertexArray& array, uint32_t size) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

	public:
		static void SendDebugMessage(const char* message, const char* code);
		static ErrorCallback s_Callback;

	private:
		VulkanData m_VulkanData;
	};
}
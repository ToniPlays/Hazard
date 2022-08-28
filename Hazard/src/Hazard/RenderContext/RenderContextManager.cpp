
#include <hzrpch.h>
#include "RenderContextManager.h"
#include "Hazard/Core/HazardLoop.h"

namespace Hazard 
{
	using namespace HazardRenderer;

	RenderContextManager::RenderContextManager(RenderContextCreateInfo* createInfo) : Module("RenderContextManager")
	{
		//Create window and initialize

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hazard";
		appInfo.BuildVersion = HZR_BUILD_VERSION;
		appInfo.EventCallback = [](Event& e) { HazardLoop::GetCurrent().OnEvent(e); };
		appInfo.MessageCallback = [](RenderMessage message) { std::cout << message.Description << std::endl; };

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = "Hazard engine";
		windowInfo.Width = 1280;
		windowInfo.Height = 720;
		windowInfo.Color = { 0, 1, 0.5, 1.0 };

		HazardRendererCreateInfo rendererInfo = {};
		rendererInfo.pAppInfo = &appInfo;
		rendererInfo.Logging = true;
		rendererInfo.Renderer = createInfo->Renderer;
		rendererInfo.VSync = createInfo->VSync;
		rendererInfo.WindowCount = 1;
		rendererInfo.pWindows = &windowInfo;

		m_Window = Window::Create(&rendererInfo);
		m_Window->Show();

	}
	void RenderContextManager::PreRender()
	{
		HZR_PROFILE_FUNCTION();
		//Prepare to render frame
		Renderer::BeginFrame();
	}
	void RenderContextManager::PostRender()
	{
		HZR_PROFILE_FUNCTION();
		//Renderer from queue
		m_Window->BeginFrame();

		auto& swapchain = m_Window->GetSwapchain();
		auto& commandBuffer = swapchain->GetSwapchainBuffer();
		commandBuffer->BeginRenderPass(swapchain->GetRenderPass());
		commandBuffer->EndRenderPass();

		Renderer::WaitAndRender();
		m_Window->Present();
	}
}

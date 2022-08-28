
#include <hzrpch.h>
#include "RenderContextManager.h"
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Assets/AssetManager.h"
#include "ImageAssetLoader.h"

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
		windowInfo.Title = createInfo->Title;
		windowInfo.Width = createInfo->Width;
		windowInfo.Height = createInfo->Height;
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

		AssetManager::RegisterLoader<ImageAssetLoader>(AssetType::Image);

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
		Renderer::WaitAndRender();
		m_Window->Present();
	}
}


#include <hzrpch.h>
#include "RenderContextManager.h"
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Assets/AssetManager.h"
#include "ImageAssetLoader.h"
#include "PipelineAssetLoader.h"
#include "ShaderAssetLoader.h"

#include "ShaderLibrary.h"

namespace Hazard 
{
	using namespace HazardRenderer;

	RenderContextManager::RenderContextManager(ApplicationCreateInfo* appInfo, RenderContextCreateInfo* createInfo) : Module("RenderContextManager")
	{
		HZR_PROFILE_FUNCTION();
		//Create window and initialize
		HZR_CORE_ASSERT(Window::IsRenderAPISupported(createInfo->Renderer), "Selected RenderAPI not supported");

		HazardRendererAppInfo hzrAppInfo = {};
		hzrAppInfo.AppName = "Hazard";
		hzrAppInfo.BuildVersion = HZR_BUILD_VERSION;
		hzrAppInfo.EventCallback = [](Event& e) { HazardLoop::GetCurrent().OnEvent(e); };

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = createInfo->Title;
		windowInfo.Extent.Width = createInfo->Width;
		windowInfo.Extent.Height = createInfo->Height;
		windowInfo.Color = { 0, 1, 0.5, 1.0 };

		HazardRendererCreateInfo rendererInfo = {};
		rendererInfo.pAppInfo = &hzrAppInfo;
		rendererInfo.Logging = appInfo->Logging;
		rendererInfo.Renderer = createInfo->Renderer;
		rendererInfo.VSync = createInfo->VSync;
		rendererInfo.WindowCount = 1;
		rendererInfo.pWindows = &windowInfo;

		m_Window = Window::Create(&rendererInfo);
		m_Window->Show();

		AssetManager::RegisterLoader<ImageAssetLoader>(AssetType::Image);
		//AssetManager::RegisterLoader<PipelineAssetLoader>(AssetType::Pipeline);
		AssetManager::RegisterLoader<ShaderAssetLoader>(AssetType::Shader);
	}
	void RenderContextManager::PreRender()
	{
		HZR_PROFILE_FUNCTION();
		//Prepare to render frame
		Input::Update();
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

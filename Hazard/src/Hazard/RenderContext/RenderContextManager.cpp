
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

		HazardRendererAppInfo hzrAppInfo = {
			.AppName = "Hazard",
			.BuildVersion = HZR_BUILD_VERSION,
			.EventCallback = [](Event& e) { HazardLoop::GetCurrent().OnEvent(e); }
		};

		HazardWindowCreateInfo windowInfo = {
			.Title = createInfo->Title,
			.Extent = { createInfo->Width, createInfo->Height, 1 },
			.Color = Color::Black,
		};

		HazardRendererCreateInfo rendererInfo = {
			.pAppInfo = &hzrAppInfo,
			.Renderer = createInfo->Renderer,
			.WindowCount = 1,
			.pWindows = &windowInfo,
			.VSync = createInfo->VSync
		};

		m_Window = Window::Create(&rendererInfo);
		m_Window->Show();

		AssetManager::RegisterLoader<ImageAssetLoader>(AssetType::Image);
		AssetManager::RegisterLoader<ShaderAssetLoader>(AssetType::Shader);

		m_Window->AddDebugCallback([this](const RenderMessage& message) {
			switch (message.Severity)
			{
				case Severity::Error:
					HZR_CORE_ERROR("Validation error: {}", message.Description);
					HZR_CORE_ERROR("Details: {}", message.StackTrace);
					break;
			}
		});
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

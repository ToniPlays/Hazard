#if 0
#include "HazardLauncher.h"
#include "HazardRendererCore.h"

#include "GUIManager.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;

void HazardLauncher::PreInit()
{

	std::filesystem::current_path("C:/dev/Hazard/HazardLauncher");

	RenderAPI renderAPI = RenderAPI::OpenGL;
	ApplicationCreateInfo appInfo = {};
	appInfo.AppName = "Hazard Launcher";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;

	HazardRendererAppInfo rendererApp = {};
	rendererApp.AppName = appInfo.AppName;
	rendererApp.BuildVersion = HZR_BUILD_VERSION;
	rendererApp.IconCount = 0;
	rendererApp.pIcons = nullptr;

	rendererApp.EventCallback = [&](Event& e) {
		HazardLoop::GetCurrent().OnEvent(e);
	};
	HazardWindowCreateInfo window = {};
	window.Title = "Hazard Launcher";
	window.FullScreen = false;
	window.Maximized = false;
	window.Resizable = false;
	window.HasTitlebar = true;

	window.Width = 1280;
	window.Height = 720;
	window.Color = Color::FromHex("#323232");

	HazardRendererCreateInfo rendererInfo = {};
	rendererInfo.pAppInfo = &rendererApp;
	rendererInfo.Renderer = renderAPI;
	rendererInfo.VSync = true;
	rendererInfo.WindowCount = 1;
	rendererInfo.pWindows = &window;

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RendererInfo = &rendererInfo;

	CreateApplicationStack(&createInfo);
}

void HazardLauncher::Init()
{
	PushModule<GUIManager>();
}
#endif
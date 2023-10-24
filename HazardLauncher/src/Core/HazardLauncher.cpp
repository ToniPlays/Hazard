
#include "HazardLauncher.h"
#include "HazardRendererCore.h"

#include "GUIManager.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;

void HazardLauncher::PreInit()
{
	ApplicationCreateInfo appInfo = {};
	appInfo.AppName = "Hazard Launcher";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;
	appInfo.ThreadCount = 1;

	RenderContextCreateInfo renderContextInfo = {};
	renderContextInfo.Renderer = RenderAPI::Auto;
	renderContextInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderContextInfo.Title = "Hazard Editor | " + RenderAPIToString(renderContextInfo.Renderer);
	renderContextInfo.Width = 1280;
	renderContextInfo.Height = 720;

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &renderContextInfo;

	CreateApplicationStack(&createInfo);
}

void HazardLauncher::Init()
{
	PushModule<GUIManager>();
}

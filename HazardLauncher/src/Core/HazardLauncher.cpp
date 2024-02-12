
#include "HazardLauncher.h"
#include "HazardRendererCore.h"

#include "GUIManager.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;

void HazardLauncher::PreInit()
{
	ApplicationCreateInfo appInfo = {
		.AppName = "Hazard Launcher",
		.BuildVersion = HZR_BUILD_VERSION,
		.Logging = true,
		.MaxJobThreads = 1,
	};

	RenderContextCreateInfo renderContextInfo = {
		.Renderer = RenderAPI::Auto,
		.Title = fmt::format("Hazard Editor | {}", RenderAPIToString(renderContextInfo.Renderer)),
		.Width = 1280,
		.Height = 720,
		.VSync = CommandLineArgs::Get<bool>("VSync"),
	};

	HazardCreateInfo createInfo = {
		.AppInfo = &appInfo,
		.RenderContextInfo = &renderContextInfo,
	};

	CreateApplicationStack(&createInfo);
}

void HazardLauncher::Init()
{
	PushModule<GUIManager>();
}

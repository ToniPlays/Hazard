
#include "HazardLauncher.h"
#include "HazardRendererCore.h"
#include "Hazard/ImGUI/GUIManager.h"
#include "GUI/LauncherGUI.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;

void HazardLauncher::PreInit()
{
	ApplicationCreateInfo appInfo = {
		.AppName = "Hazard Launcher",
		.BuildVersion = HZR_BUILD_VERSION,
		.Logging = true,
		.MaxWorkers = 1,
	};

	RenderContextCreateInfo renderContextInfo = {
		.Renderer = RenderAPI::Auto,
		.Title = fmt::format("Hazard Editor | {}", RenderAPIToString(renderContextInfo.Renderer)),
		.Width = 1280,
		.Height = 720,
		.VSync = CommandLineArgs::Get<bool>("VSync"),
	};

	GuiCreateInfo guiInfo = {
		.Title = appInfo.AppName,
		.DockSpace = false
	};

	HazardCreateInfo createInfo = {
		.AppInfo = &appInfo,
		.RenderContextInfo = &renderContextInfo,
		.GuiInfo = &guiInfo
	};

	CreateApplicationStack(&createInfo);
}

void HazardLauncher::Init()
{
	using namespace Hazard::ImUI;
	auto& manager = GetModule<GUIManager>();
	manager.New<LauncherGUI>();
}

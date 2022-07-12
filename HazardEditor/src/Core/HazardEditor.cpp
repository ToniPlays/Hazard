
#include "hzrpch.h"
#include "HazardEditor.h"
#include "Editor/EditorCamera.h"
#include "HazardRenderer.h"
#include "GUIManager.h"

using namespace Hazard;


void EditorApplication::PreInit()
{
	RenderAPI renderAPI = RenderAPI::OpenGL;

    
#ifdef HZR_PLATFORM_MACOS
        std::filesystem::current_path("/users/ToniSimoska/Hazard/HazardEditor");
    renderAPI = RenderAPI::Metal;
#endif
    

	HZR_INFO("EditorApplication::PreInit()");
	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;
	
	HazardRendererAppInfo rendererApp = {};
	rendererApp.AppName = appInfo.AppName;
	rendererApp.BuildVersion = HZR_BUILD_VERSION;
	rendererApp.IconCount = 2;
	rendererApp.pIcons = icons.data();
	rendererApp.EventCallback = [&](Event& e) {
		HazardLoop::GetCurrent().OnEvent(e);
	};
	rendererApp.MessageCallback = [](RenderMessage message) {
		switch (message.Severity) {
		case Severity::Debug:		HZR_CORE_INFO(message.Message);		break;
		case Severity::Trace:		HZR_CORE_TRACE(message.Message);	break;
		case Severity::Info:		HZR_CORE_INFO(message.Message);		break;
		case Severity::Warning:		HZR_CORE_WARN(message.Message);		break;
		case Severity::Error:		HZR_CORE_ERROR(message.Message);	break;
		case Severity::Critical:	HZR_CORE_FATAL(message.Message);	break;
		}
	};

	HazardWindowCreateInfo windowInfo = {};
	windowInfo.Title = "HazardEditor | " + RenderAPIToString(renderAPI);
	windowInfo.FullScreen = false;
	windowInfo.Maximized = false;
	windowInfo.Decorated = true;
#ifdef HZR_PLATFORM_MACOS
	windowInfo.Width = 2880;
	windowInfo.Height = 1600;
#else
	windowInfo.Width = 1920;
	windowInfo.Height = 1080;
#endif
	windowInfo.Color = Color(34, 34, 34, 255);

	HazardRendererCreateInfo renderInfo = {};
	renderInfo.pAppInfo = &rendererApp;
	renderInfo.Renderer = renderAPI;
	renderInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderInfo.WindowCount = 1;
	renderInfo.pWindows = &windowInfo;

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RendererInfo = &renderInfo;

	CreateApplicationStack(&createInfo);
}
void EditorApplication::Init()
{
	PushModule<GUIManager>();
}

bool EditorApplication::OnEvent(Event& e)
{
	return false;
}

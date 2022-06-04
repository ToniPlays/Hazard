
#include "hzrpch.h"
#include "HazardEditor.h"
#include "Editor/EditorCamera.h"
#include "HazardRenderer.h"

using namespace Hazard;


void EditorApplication::PreInit()
{
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
	windowInfo.Title = "HazardEditor";
	windowInfo.FullScreen = false;
	windowInfo.Maximized = false;
	windowInfo.Decorated = true;
	windowInfo.Width = 1920;
	windowInfo.Height = 1080;
	windowInfo.Color = Color(34, 34, 34, 255);

	HazardRendererCreateInfo renderInfo = {};
	renderInfo.pAppInfo = &rendererApp;
	renderInfo.Renderer = RenderAPI::Vulkan;
	renderInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderInfo.WindowCount = 1;
	renderInfo.pWindows = &windowInfo;

	/*
	AudioEngineCreateInfo audioInfo = {};

	ScriptEngineCreateInfo scriptInfo = {};
#ifdef HZR_PLATFORM_WINDOWS
	scriptInfo.AppAssemblyPath = CommandLineArgs::Get<std::string>("AppCore");
	scriptInfo.CoreAssemblyPath = "C:/dev/Hazard/HazardScripting/bin/debug/HazardScripting.dll";
	scriptInfo.MonoDirectoryLib = "C:/Program Files/Mono/lib";
	scriptInfo.MonoDirectoryEtc = "C:/Program Files/Mono/etc/";

	
#else
	scriptInfo.MonoDirectoryLib = "/usr/local/Cellar/mono/6.12.0.122/lib/";
	scriptInfo.MonoDirectoryEtc = "/usr/local/Cellar/mono/6.12.0.122/etc/";
#endif
	scriptInfo.Enable = false;

	EntityComponentCreateInfo entityInfo = {};
	*/
	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RendererInfo = &renderInfo;
	/*
    createInfo.RendererInfo = &engineInfo;
	createInfo.AudioEngine = &audioInfo;
	createInfo.EntityComponent = &entityInfo;
	createInfo.ScriptEngineInfo = &scriptInfo;
	*/

	CreateApplicationStack(&createInfo);
}
void EditorApplication::Init()
{
/*
#ifdef HZR_PLATFORM_WINDOWS

	PushModule<WindowElement::EditorView>();
#endif
    Runtime::SceneRuntimeHandler::Init();

    Project::ProjectManager& manager = PushModule<Project::ProjectManager>();
	manager.Load(CommandLineArgs::Get<std::string>("ProjectPath"));
	*/
}

bool EditorApplication::OnEvent(Event& e)
{
	
	//if (!HasModule<WindowElement::EditorView>()) return false;
	//return GetModule<WindowElement::EditorView>().OnEvent(e);
	return false;
}

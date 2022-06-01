
#include "hzrpch.h"
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "SceneRuntimeHandler.h"
#include "Project/ProjectManager.h"
#include "Editor/EditorCamera.h"

using namespace Hazard;


void EditorApplication::PreInit()
{
	/*std::vector<const char*> icons = {"res/Icons/logo.png", "res/Icons/logo.png"};

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;
	appInfo.IconCount = icons.size();
	appInfo.Icons = icons.data();

	RenderContexCreateInfo contextInfo = {};
	contextInfo.Renderer = CommandLineArgs::Get<RenderAPI>("RenderAPI");
	contextInfo.FullScreen = false;
	contextInfo.Maximized = false;
	contextInfo.Decorated = true;
	contextInfo.Logging = false;
	contextInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	contextInfo.Width = 1920;
	contextInfo.Height = 1080;

	RenderEngineCreateInfo engineInfo = {};
	engineInfo.MaxQuadCount = 15000;
	engineInfo.SamplerCount = 32;
	engineInfo.Enabled = true;

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

	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &contextInfo;
    createInfo.RendererInfo = &engineInfo;
	createInfo.AudioEngine = &audioInfo;
	createInfo.EntityComponent = &entityInfo;
	createInfo.ScriptEngineInfo = &scriptInfo;
	*/
	HazardCreateInfo createInfo = {};

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

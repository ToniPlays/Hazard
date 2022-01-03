#pragma once

#include "hzrpch.h"
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "SceneRuntimeHandler.h"
#include "Project/ProjectManager.h"

using namespace Hazard;

void EditorApplication::PreInit()
{
	HZR_PROFILE_FUNCTION();
	std::vector<const char*> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;
	appInfo.IconCount = icons.size();
	appInfo.Icons = icons.data();

	RenderContexCreateInfo contextInfo = {};
	contextInfo.Renderer = RenderAPI::OpenGL;
	contextInfo.ImagesInFlight = 3;
	contextInfo.FullScreen = false;
	contextInfo.Maximized = false;
	contextInfo.Decorated = true;
	contextInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	contextInfo.Width = 1280;
	contextInfo.Height = 720;

	RenderEngineCreateInfo engineInfo = {};
	engineInfo.MaxQuadCount = 15000;
	engineInfo.SamplerCount = 32;
	engineInfo.Enabled = true;

	AudioEngineCreateInfo audioInfo = {};

	ScriptEngineCreateInfo scriptInfo = {};
#ifdef HZR_PLATFORM_WINDOWS
	scriptInfo.AppAssemblyPath = "c:/dev/HazardCraft/Assets/Scripts/Binaries/HazardCraft.dll";
	scriptInfo.CoreAssemblyPath = "c:/dev/Hazard/HazardScripting/bin/debug/HazardScripting.dll";
#else
	scriptInfo.MonoDirectoryLib = "/usr/local/Cellar/mono/6.12.0.122/lib/";
	scriptInfo.MonoDirectoryEtc = "/usr/local/Cellar/mono/6.12.0.122/etc/";
#endif
	scriptInfo.Enable = false;

	EntityComponentCreateInfo entityInfo = {};

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &contextInfo;
	createInfo.RendererInfo = &engineInfo;
	createInfo.AudioEngine = &audioInfo;
	createInfo.ScriptEngineInfo = &scriptInfo;
	createInfo.EntityComponent = &entityInfo;

	CreateApplicationStack(&createInfo);
}
void EditorApplication::Init()
{
	HZR_PROFILE_FUNCTION();
	PushModule<WindowElement::EditorView>();
	Runtime::SceneRuntimeHandler::Init();

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();
	manager.Load("C:/dev/HazardProject/Hazard.hzrproj");
}

bool EditorApplication::OnEvent(Event& e)
{
	if (!HasModule<WindowElement::EditorView>()) return false;
	return GetModule<WindowElement::EditorView>().OnEvent(e);
}

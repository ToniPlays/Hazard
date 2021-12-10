#pragma once

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
	contextInfo.Renderer = RenderAPI::Vulkan;
	contextInfo.ImagesInFlight = 2;
	contextInfo.FullScreen = false;
	contextInfo.Maximized = false;
	contextInfo.Decorated = true;
	contextInfo.VSync = true; 
	contextInfo.Color = "#323232";
	contextInfo.Width = 1280;
	contextInfo.Height = 720;

	RenderEngineCreateInfo engineInfo = {};
	engineInfo.MaxQuadCount = 15000;
	engineInfo.SamplerCount = 32;

	AudioEngineCreateInfo audioInfo = {};

	ScriptEngineCreateInfo scriptInfo = {};
	scriptInfo.AppAssemblyPath = "c:/dev/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll";
	scriptInfo.CoreAssemblyPath = "c:/dev/Hazard/HazardScripting/bin/debug/netstandard2.0/HazardScripting.dll";
	scriptInfo.MonoDirectoryLib = "C:/Program Files/Mono/Lib";
	scriptInfo.MonoDirectoryEtc = "C:/Program Files/Mono/Etc";
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

#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "Project/ProjectManager.h"
#include "GUI/Window/AllWindows.h"
#include "SceneRuntimeHandler.h"

void EditorApplication::PreInit()
{
	HZR_PROFILE_FUNCTION();
	std::vector<std::string> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.appName = "Hazard Editor";
	appInfo.buildVersion = HZR_BUILD_VERSION;
	appInfo.logging = false;
	appInfo.iconCount = icons.size();
	appInfo.icons = icons.data();

	RenderContexCreateInfo contextInfo;
	contextInfo.renderer = RenderAPI::Vulkan;
	contextInfo.imagesInFlight = 2;
	contextInfo.fullScreen = false;
	contextInfo.maximized = true;
	contextInfo.decorated = true;
	contextInfo.VSync = false;
	contextInfo.color = { 0.06f, 0.06f, 0.06f };
	contextInfo.width = 1280;
	contextInfo.height = 720;

	RenderEngineCreateInfo engineInfo;
	engineInfo.maxQuadCount = 50000;
	engineInfo.samplerCount = 8;
	engineInfo.shaderSourcePath = "res/shaders/compiled";
	engineInfo.shaderCompilePath = "res/shaders/compiled";

	AudioEngineCreateInfo audioInfo;

	ScriptEngineCreateInfo scriptInfo;
	scriptInfo.appAssemblyPath = "c:/dev/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll";
	scriptInfo.coreAssemblyPath = "c:/dev/Hazard/HazardScripting/bin/debug/netstandard2.0/HazardScripting.dll";
	scriptInfo.monoDirectory = "C:/Program Files/Mono";
	scriptInfo.enable = false;

	EntityComponentCreateInfo entityInfo;

	HazardCreateInfo createInfo;
	createInfo.appInfo = &appInfo;
	createInfo.renderContextInfo = &contextInfo;
	createInfo.rendererInfo = &engineInfo;
	createInfo.audioEngine = &audioInfo;
	createInfo.scriptEngineInfo = &scriptInfo;
	createInfo.entityComponent = &entityInfo;

	CreateApplicationStack(&createInfo);

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();
	manager.Load("C:/dev/HazardProject/Hazard.hzrproj");

	PushModule<WindowElement::EditorView>();

}
void EditorApplication::Init()
{
	HZR_PROFILE_FUNCTION();
	Runtime::SceneRuntimeHandler::Init();
}

bool EditorApplication::OnEvent(Event& e)
{
	return GetModule<WindowElement::EditorView>().OnEvent(e);
}

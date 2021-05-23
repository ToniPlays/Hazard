#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "Project/ProjectManager.h"
#include "GUI/Window/AllWindows.h"
#include "SceneRuntimeHandler.h"

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::PreInit()
{
	HZR_PROFILE_FUNCTION();
	std::vector<std::string> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.appName = "Hazard Editor";
	appInfo.buildVersion = HZR_BUILD_VERSION;
	appInfo.logging = true;
	appInfo.iconCount = icons.size();
	appInfo.icons = icons.data();

	RenderContexCreateInfo contextInfo;
	contextInfo.renderer = RenderAPI::OpenGL;
	contextInfo.fullScreen = true;
	contextInfo.color = { 1, 1, 1 };
	contextInfo.VSync = true;
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

	HazardCreateInfo createInfo;
	createInfo.appInfo = &appInfo;
	createInfo.renderContextInfo = &contextInfo;
	createInfo.rendererInfo = &engineInfo;
	createInfo.audioEngine = &audioInfo;
	createInfo.scriptEngineInfo = &scriptInfo;

	CreateApplicationStack(&createInfo);

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();
	if (!manager.Load("C:\\dev\\HazardProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");
	}
	
	PushModule<ECS::SceneHandler>();
	PushModule<WindowElement::EditorView>().GetRenderable<WindowElement::FileView>()->
		SetRootPath(manager.GetProject().m_AbsolutePath.c_str());
}
void EditorApplication::Init() 
{
	HZR_PROFILE_FUNCTION();
	Project::ProjectManager manager = *GetModule<Project::ProjectManager>();
	GetModule<ECS::SceneHandler>()->LoadScene(manager.GetProject().m_StartupScene.c_str(), ECS::Serialization::Editor);
	Runtime::SceneRuntimeHandler::Init();
}

bool EditorApplication::OnEvent(Event& e)
{
	return GetModule<WindowElement::EditorView>()->OnEvent(e);
}




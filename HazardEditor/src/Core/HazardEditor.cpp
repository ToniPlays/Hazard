#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "Project/ProjectManager.h"
#include "GUI/Window/AllWindows.h"
#include "SceneRuntimeHandler.h"

RenderAPI api = RenderAPI::OpenGL;

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::PreInit()
{
	std::vector<std::string> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.appName = "Hazard Editor";
	appInfo.buildVersion = HZR_BUILD_VERSION;
	appInfo.logging = false;
	appInfo.icons = icons.data();
	appInfo.iconCount = icons.size();

	RenderContexCreateInfo contextInfo;
	contextInfo.renderer = RenderAPI::OpenGL;
	contextInfo.fullScreen = true;
	contextInfo.width = 1280;
	contextInfo.height = 720;

	RenderEngineCreateInfo engineInfo;
	engineInfo.maxQuadCount = 50000;

	HazardCreateInfo createInfo;
	createInfo.appInfo = &appInfo;
	createInfo.renderContextInfo = &contextInfo;
	createInfo.rendererInfo = &engineInfo;

	CreateApplicationStack(&createInfo);

	PushModule<ScriptEngineManager>();

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();
	if (!manager.Load("C:\\dev\\HazardProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");
	}
	
	PushModule<WindowElement::EditorView>().GetRenderable<WindowElement::FileView>()->
		SetRootPath(manager.GetProject().m_AbsolutePath.c_str());
	PushModule<ECS::SceneHandler>();
	
}
void EditorApplication::Init() 
{
	Project::ProjectManager manager = *GetModule<Project::ProjectManager>();
	GetModule<ECS::SceneHandler>()->LoadScene(manager.GetProject().m_StartupScene.c_str(), ECS::Serialization::Editor);
	Runtime::SceneRuntimeHandler::Init();
}

bool EditorApplication::OnEvent(Event& e)
{
	return GetModule<WindowElement::EditorView>()->OnEvent(e);
}




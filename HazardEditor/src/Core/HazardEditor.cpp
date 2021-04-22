#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "Project/ProjectManager.h"
#include "GUI/Window/AllWindows.h"

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::PreInit()
{
	PushModule<Logging::Logger>();
	PushModule<Rendering::RenderContext>();

	std::stringstream ss;
	ss << "Hazard Editor | Hazard "; 
	ss << HZR_BUILD_VERSION;
	ss << " for " << HZR_PLATFORM;
	ss << " (" << HAZARD_SYSTEM_BIT << ")";

	SetTitle(ss.str());
	SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");
	PushModule<Rendering::RenderEngine>();
	PushModule<Scripting::ScriptEngineManager>();

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();

	if (!manager.Load("C:\\dev\\HazardProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");
	}

	PushModule<ECS::SceneHandler>();

	PushModule<WindowElement::EditorView>().GetRenderable<WindowElement::FileView>().
		SetRootPath(manager.GetProject().absolutePath.c_str());
	
}
void EditorApplication::Init() 
{
	Project::ProjectManager manager = GetModule<Project::ProjectManager>();
	GetModule<ECS::SceneHandler>().LoadScene(manager.GetProject().startupScene.c_str(), ECS::Serialization::Editor);
}

bool EditorApplication::OnEvent(Event& e)
{
	 return GetModule<WindowElement::EditorView>().OnEvent(e);
}




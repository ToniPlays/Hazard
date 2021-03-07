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
	PushModule<Scripting::ScriptEngine>().InitAssembly("c:/dev/HazardTestProject/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll");

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();

	if (!manager.Load("C:\\dev\\HazardTestProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");
	}

	PushModule<ECS::SceneHandler>().LoadScene(manager.GetProject().startupScene.c_str(), ECS::Serialization::Editor);
	PushModule<WindowElement::EditorView>().GetRenderable<WindowElement::FileView>().
		SetRootPath(manager.GetProject().absolutePath.c_str());
}
void EditorApplication::Init() 
{

}

bool EditorApplication::OnEvent(Event& e)
{
	 return GetModule<WindowElement::EditorView>().OnEvent(e);
}




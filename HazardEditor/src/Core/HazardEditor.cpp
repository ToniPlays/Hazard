#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"
#include "Project/ProjectManager.h"

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::Init()
{
	PushModule<Logging::Logger>();
	Rendering::RenderContext context = PushModule<Rendering::RenderContext>();

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();

	if (!manager.Load("C:\\dev\\HazardTestProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");

	}
	std::stringstream ss;
	ss << manager.GetProject().name << " | Hazard " << HZR_BUILD_VERSION << " (" << HAZARD_SYSTEM_BIT << ")" << " for " << HZR_PLATFORM;

	context.SetClearColor(Color::FromHex("#222222"));
	context.GetWindow().SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");

	PushModule<Rendering::RenderEngine>();
	PushModule<ECS::SceneHandler>().LoadScene((manager.GetProject().absolutePath + "\\" + 
		manager.GetProject().startupScene + ".hazard").c_str(), ECS::Serialization::Editor);


	context.GetWindow().SetWindowTitle(ss.str().c_str());
	PushModule<WindowElement::EditorView>();

}




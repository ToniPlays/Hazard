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
	Rendering::RenderContext context = PushModule<Rendering::RenderContext>();
	std::stringstream ss;
	ss << "Hazard Editor | Hazard " << HZR_BUILD_VERSION << " (" << HAZARD_SYSTEM_BIT << ")" << " for " << HZR_PLATFORM;

	context.GetWindow().SetWindowTitle(ss.str().c_str());

	context.SetClearColor(Color::FromHex("#222222"));
	context.GetWindow().SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");
	PushModule<Rendering::RenderEngine>();

	Project::ProjectManager& manager = PushModule<Project::ProjectManager>();

	if (!manager.Load("C:\\dev\\HazardTestProject\\Hazard.hzrproj")) {
		HZR_ERROR("Valid project file required to run application");
	}

	PushModule<ECS::SceneHandler>().LoadScene(manager.GetProject().startupScene.c_str(), ECS::Serialization::Editor);

	PushModule<WindowElement::EditorView>().GetRenderable<WindowElement::FileView>().
		SetRootPath(manager.GetProject().absolutePath.c_str());


}
void EditorApplication::Init() {
	
}

bool EditorApplication::OnEvent(Event& e)
{
	 return Core::HazardLoop::GetModule<WindowElement::EditorView>().OnEvent(e);
}




#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "GUI/EditorView.h"

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

	std::stringstream ss;
	ss << "Hazard Editor | Hazard " << HZR_BUILD_VERSION << " (" << HAZARD_SYSTEM_BIT << ")" << " for " << HZR_PLATFORM;
	context.SetClearColor(Color::FromHex("#222222"));
	context.GetWindow().SetWindowTitle(ss.str().c_str());
	context.GetWindow().SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");

	PushModule<Rendering::RenderEngine>();
	PushModule<ECS::SceneHandler>();

	PushModule<WindowElement::EditorView>();
}




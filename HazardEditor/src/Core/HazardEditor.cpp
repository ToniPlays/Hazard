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
	SetTitle("HazardEditor");
	PushModule<Logging::Logger>();
	PushModule<Rendering::RenderContext>();
	PushModule<Rendering::RenderEngine>();
	PushModule<ECS::SceneHandler>();

	PushModule<WindowElement::EditorView>();
}




#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"


using namespace Hazard;

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}


void EditorApplication::Awake()
{
#ifndef HZR_GAME_ONLY
	PushModule(new Editor());
#endif
}

void EditorApplication::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazard::MouseScrolledEvent>(BIND_EVENT(EditorApplication::CameraZoom));
}

bool EditorApplication::CameraZoom(Event& e) 
{
	return true;
}

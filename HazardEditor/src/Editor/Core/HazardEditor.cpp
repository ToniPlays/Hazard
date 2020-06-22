#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"

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

void EditorApplication::OnEvent(Hazard::Event& e)
{
}

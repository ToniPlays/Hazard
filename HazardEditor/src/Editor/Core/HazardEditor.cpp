#pragma once
#include "HazardEditor.h"
#include "Editor/GUI/Editor.h"

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

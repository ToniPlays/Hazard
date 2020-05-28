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
	PushModule(new Editor());

}

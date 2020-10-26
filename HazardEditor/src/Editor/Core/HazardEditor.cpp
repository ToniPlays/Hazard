#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "Analytics/Debug.h"
#include "HazardProject.h"


using namespace Hazard;

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

void EditorApplication::OnEvent(Event& e)
{

}



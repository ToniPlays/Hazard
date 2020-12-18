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
	Hazard::ModuleHandler::GetModule<RenderEngine>()->GetWindow().SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");
	PushModule(new Editor());

}
void EditorApplication::Update()
{

}

bool EditorApplication::OnEvent(Event& e)
{
	return true;
}
void EditorApplication::OnClose() {
	
}



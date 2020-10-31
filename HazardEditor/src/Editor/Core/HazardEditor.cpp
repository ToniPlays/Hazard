#pragma once
#include <hzreditor.h>
#include "HazardEditor.h"
#include "Analytics/Debug.h"
#include "HazardProject.h"

#include <glm/gtc/type_ptr.hpp>


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
	std::string project = "C:\\dev\\HazardTestProject\\TestProject.hzrproj";
	HazardProject::OpenProject(project);

}
void EditorApplication::Update()
{

}

void EditorApplication::OnEvent(Event& e)
{

}



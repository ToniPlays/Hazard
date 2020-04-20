#pragma once
#include <hzrpch.h>
#include "EditorGUI.h"
#include "imgui.h"
#include "Items/MainMenu.h"

namespace Hazard {
	EditorGUI::EditorGUI() : Hazard::Layer("EditorLayer")
	{

	}

	EditorGUI::~EditorGUI()
	{

	}
	void EditorGUI::OnAttach()
	{

	}

	void EditorGUI::OnImGuiRender()
	{
		MainMenu::Render();
	}
}


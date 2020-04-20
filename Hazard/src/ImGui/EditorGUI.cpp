#pragma once
#include <hzrpch.h>
#include "EditorGUI.h"
#include "imgui.h"
#include "Widgets/Items/MainMenu.h"
#include "Widgets/GuiStats.h"

namespace Hazard {
	ColorPicker* EditorGUI::colorPicker;

	EditorGUI::EditorGUI() : Hazard::Layer("EditorLayer")
	{

	}

	EditorGUI::~EditorGUI()
	{
		
	}
	void EditorGUI::OnAttach()
	{
		colorPicker = new ColorPicker();
		PushLayer(new MainMenu());
		PushLayer(new GuiStats());
	}

	void EditorGUI::OnDetach()
	{
		for (GuiLayer* layer : layers) {
			delete layer;
		}
	}

	void EditorGUI::OnImGuiRender()
	{
		for (GuiLayer* layer : layers) {
			layer->OnRender();
		}
		if (colorPicker->isOpen) colorPicker->OnRender();
	}
	void EditorGUI::OpenPicker(GuiLayer* layer)
	{
		colorPicker->getterLayer = layer;
		colorPicker->isOpen = true;
	}
}


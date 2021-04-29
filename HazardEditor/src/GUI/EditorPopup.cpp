#pragma once

#include <hzreditor.h>
#include "EditorPopup.h"
#include "EditorView.h"

namespace WindowElement {

	EditorPopup::EditorPopup(const char* title) : m_Title(title)
	{

	}

	EditorPopup::~EditorPopup()
	{

	}
	void EditorPopup::OnRender()
	{
		if (!open)
			EditorView::GetInstance().RemoveRenderable<EditorPopup>();

		ImVec2 windowCenter = ImGui::GetMainViewport()->GetCenter();

		ImGui::OpenPopup(m_Title);
		ImGui::SetNextWindowSize(m_Size);
		ImGui::SetNextWindowPos(windowCenter, 0, {0.5f, 0.65f });

		if(!ImGui::BeginPopupModal(m_Title, &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_Modal))
			return;

		OnWindowRender();
		ImGui::EndPopup();
	}
}
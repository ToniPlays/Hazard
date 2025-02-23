#pragma once

#include "Hazard.h"
#include "Core/EditorEvent.h"
#include "Editor/EditorUtils.h"
#include "Hazard/ImGUI/Panel.h"

#include "ImTextEdit.h"

namespace UI
{
	class TextEditPanel : public Hazard::ImUI::Panel
	{
	public:
		TextEditPanel();

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		void OpenFile();
		void OpenFile(const std::filesystem::path& path);

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		void Save();
		void SetLanguageDefinition(const std::string& extension);

	private:
		ImTextEdit m_TextEditor;
		std::filesystem::path m_Path;

		float m_TextSize = 16.0;
	};
}
#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Modal.h"
#include "Core/EditorAssetManager.h"

namespace UI
{
	class ScriptCreatePanel : public Hazard::ImUI::Modal
	{
	public:
		ScriptCreatePanel();
		~ScriptCreatePanel() = default;

		void Update() override {};
		bool OnEvent(Event& e) override;
		bool OnKeyPressed(KeyPressedEvent& e);
		void OnPanelRender() override;
		void DrawMethodList();

		void SetDirectory(const std::filesystem::path& directory) { m_Directory = directory; }

	private:
		ScriptCreateInfo m_CreateInfo;
		std::filesystem::path m_Directory;
	};
}

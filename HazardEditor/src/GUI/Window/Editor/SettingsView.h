#pragma once
#include "GUI/EditorWindow.h"
#include "imgui.h"

namespace WindowElement {

	class SettingsView : public EditorWindow {
	public:
		SettingsView();
		~SettingsView() = default;

		void OnWindowRender() override;
		void OnBeforeRender() override;
		void OnAfterRender() override;
	private:
		void CreateMenus();
		void CreateContent();

		std::string m_SearchValue;
	};
}
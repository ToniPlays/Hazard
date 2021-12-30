#pragma once
#include "GUI/EditorWindow.h"
#include "Hazard/Rendering/Texture.h"
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
		std::unordered_map<std::string, Ref<Hazard::Rendering::Texture2D>> m_Images;
	};
}
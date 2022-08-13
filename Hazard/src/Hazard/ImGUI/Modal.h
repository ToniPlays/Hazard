#pragma once

#include "GUIRenderable.h"
#include "UILibrary.h"
#include "ScopedVar.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	class Modal : public GUIRenderable {
	public:

		Modal() = default;
		Modal(const std::string& title, ImVec2 size) : m_Title(title), m_Size(size) {}

		virtual ~Modal() = default;

		void Open() { m_Open = true; };
		void Close() { m_Open = false; };

		//<summary>
		// This renders the panel
		//</summary>

		void Render() override
		{
			if (!m_Open) return;

			ImGui::OpenPopup(m_Title.c_str());

			const Style& style = StyleManager::GetCurrent();

			ImVec2 size = ImGui::GetMainViewport()->Size;
			ImGui::SetNextWindowSize(m_Size);
			ImGui::SetNextWindowPos({ size.x / 2.0f, size.y / 2.5f }, 0, { 0.5, 0.5 });
			ImUI::ScopedStyleColor color(ImGuiCol_PopupBg, style.ChildBackgroundColor);
			if (ImGui::BeginPopupModal(m_Title.c_str(), &m_Open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
				OnPanelRender();
				ImGui::EndPopup();
			}

		};
		//<summary>
		// Render content for panel
		//</summary>
		virtual void OnPanelRender() = 0;

	protected:
		std::string m_Title = "Undefined modal";
		bool m_Open = false;
		ImVec2 m_Size;
	};
}
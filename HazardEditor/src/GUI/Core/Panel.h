#pragma once

#include "GUIRenderable.h"
#include "ScopedVar.h"
#include <imgui.h>

namespace UI {
	class Panel : public GUIRenderable {
	public:

		Panel() = default;
		Panel(const std::string& title, bool open = true) : m_Title(title), m_Open(open) {}

		virtual ~Panel() = default;

		void Open() { m_Open = true; };
		void Close() { m_Open = false; };

		//<summary>
		// This renders the panel
		//</summary>

		void Render() override {
			if (!m_Open) return;
			{
				ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(0, 8));
				ImGui::Begin(m_Title.c_str(), &m_Open);
			}
			OnPanelRender();
			ImGui::End();
		};
		//<summary>
		// Render content for panel
		//</summary>
		virtual void OnPanelRender() = 0;

	protected:
		std::string m_Title = "Undefined panel";
		bool m_Open = true;
	};
}
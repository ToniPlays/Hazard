#pragma once

#include "GUIRenderable.h"
#include "UILibrary.h"
#include "ScopedVar.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	class Panel : public GUIRenderable
	{
	public:

		Panel() = default;
		Panel(const std::string& title, bool destroyOnClose = true) : m_Title(title), m_DestroyOnClose(destroyOnClose) {}

		virtual ~Panel() = default;

		const std::string& GetTitle() const { return m_Title; }
		
		void Render() override;

		void BringToFront()
		{
			ImGuiWindow* window = ImGui::FindWindowByName(m_Title.c_str());
			if (!window) return;

			m_IsOpen = true;
			ImGui::BringWindowToFocusFront(window);
		}

		bool MouseOverWindow()
		{
			if (GImGui->CurrentWindow == nullptr)
				return false;

			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			windowPos.x -= cursorPos.x;
			windowPos.y -= cursorPos.y;

			ImVec2 bounds = { windowPos.x + windowSize.x, windowPos.y + windowSize.y };
			return ImGui::IsMouseHoveringRect(windowPos, bounds);
		}

		//<summary>
		// Render content for panel
		//</summary>
		virtual void OnPanelRender() = 0;

	protected:
		std::string m_Title = "Undefined panel";
		bool m_Hovered = false;
		bool m_DestroyOnClose = true;
		bool m_IsOpen = true;
	};
}

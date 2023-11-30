#pragma once

#include "GUIRenderable.h"
#include "UILibrary.h"
#include "ScopedVar.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	class Panel : public GUIRenderable {
	public:

		Panel() = default;
		Panel(const std::string& title, bool open = true) : m_Title(title), m_Open(open) {}

		virtual ~Panel() = default;

        void Open() { m_Open = true; OnOpen(); };
        void Close() { m_Open = false; OnClose(); };
        
        virtual void OnOpen() {}
        virtual void OnClose() {}

		//<summary>
		// This renders the panel
		//</summary>

		void Render() override 
		{
			if (!m_Open) return;
			{
				//ScopedStyleStack padding(ImGuiStyleVar_FramePadding, ImVec2(0, 8), ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
				ImGui::Begin(m_Title.c_str(), &m_Open);

				m_Hovered = MouseOverWindow();
				//float width = Math::Max(ImGui::GetContentRegionAvailWidth(), 150.0f);
				//ShiftY(-5.0f);
				//Separator({ width, 1.0f }, style.Frame.FrameHovered);
			}
			OnPanelRender();
			ImGui::End();
		};

		bool MouseOverWindow()
		{
			if(GImGui->CurrentWindow == nullptr)
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
		bool m_Open = true;
		bool m_Hovered = false;
	};
}

#pragma once

#include "GUIRenderable.h"
#include "UILibrary.h"
#include "ScopedVar.h"
#include "GUIManager.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	class Panel : public GUIRenderable
	{
	public:

		Panel() = default;
		Panel(const std::string& title) : m_Title(title) {}

		virtual ~Panel() = default;

		void Render() override
		{
			bool open = true;
			if (!ImGui::Begin(m_Title.c_str(), &open))
			{
				ImGui::End();
				return;
			}

			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if (open && !window->Viewport->PlatformRequestClose)
			{
				m_Hovered = MouseOverWindow();
				OnPanelRender();
			}
			else
			{
				//Remove panel from list
				auto& manager = Application::Get().GetModule<GUIManager>();
				manager.Destroy(this);
			}
			ImGui::End();
		};

		void BringToFront()
		{
			ImGuiWindow* window = ImGui::FindWindowByName(m_Title.c_str());
			if (!window) return;
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
	};
}

#pragma once

#include "GUIRenderable.h"
#include "UILibrary.h"
#include "ScopedVar.h"
#include <imgui.h>

namespace Hazard::ImUI
{
	class Overlay : public GUIRenderable {
	public:

		Overlay() = default;
		Overlay(const std::string& title, ImVec2 size, ImVec2 position) : m_Title(title), m_Size(size), m_Position(position) {}

		virtual ~Overlay() = default;

		void Open() { m_Open = true; };
		void Close() { m_Open = false; };

		//<summary>
		// This renders the panel
		//</summary>

		void Render() override
		{
			if (!m_Open) return;

			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = work_pos.x + work_size.x - m_Position.x;
			window_pos.y = work_pos.y + work_size.y - m_Position.y;
			window_pos_pivot.x = 1.0f;
			window_pos_pivot.y = 1.0f;

			ImGui::SetNextWindowSize(m_Size);
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowViewport(viewport->ID);
			window_flags |= ImGuiWindowFlags_NoMove;
			ImGui::SetNextWindowBgAlpha(1.0f); // Transparent background
		
			const Style& style = StyleManager::GetCurrent();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Window.TitleBgColor);
			if (ImGui::Begin(m_Title.c_str(), &m_Open, window_flags))
			{
				OnPanelRender();
			}
			ImGui::PopStyleColor();
			ImGui::End();

		};
		//<summary>
		// Render content for panel
		//</summary>
		virtual void OnPanelRender() = 0;

	protected:
		std::string m_Title = "Undefined overlay";
		bool m_Open = false;
		ImVec2 m_Size;
		ImVec2 m_Position;
	};
}